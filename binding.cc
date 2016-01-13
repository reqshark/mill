/*

  Copyright (c) 2015-2016 Bent Cardan
  Copyright (c) 2015 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include "nan.h"

using v8::FunctionTemplate;
using v8::Function;
using v8::Number;
using v8::Boolean;
using v8::String;
using v8::Object;
using v8::Value;
using v8::Local;

using Nan::HandleScope;
using Nan::MaybeLocal;
using Nan::NewBuffer;
using Nan::Callback;
using Nan::Maybe;
using Nan::Set;
using Nan::New;
using Nan::To;

extern "C" {
#include "libmill.h"
}

#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ref.h"
#include "timer.c"

static char ipstr[IPADDR_MAXSTRLEN];

/******************************************************************************/
/*  IP address library                                                        */
/******************************************************************************/

#define IPADDR_IPV4 1
#define IPADDR_IPV6 2
#define IPADDR_PREF_IPV4 3
#define IPADDR_PREF_IPV6 4

NAN_METHOD(iplocal){
  /* default port */
  int port = 5555;

  /* set mode or go with default of zero */
  int mode = 1;
  if (info[2]->IsNumber())
    mode = To<int>(info[2]).FromJust();

  /* ip address */
  char *ip = NULL;
  if (info[0]->IsString()) {
    String::Utf8Value ip(info[0]);
    if (info[1]->IsNumber())
      port = To<int>(info[1]).FromJust();
  }

  /* check port */
  if (info[0]->IsNumber()) {
    port = To<int>(info[0]).FromJust();
    if (info[1]->IsNumber())
      mode = To<int>(info[1]).FromJust();
  }

  /* get an ipaddr */
  ipaddr ipv = iplocal(ip, port, mode);
  size_t sz = sizeof(&ipv);

  /* return a libmill ipaddr as a node buffer pointer */
  Local<Object> addr = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(addr), &ipv, sz);
  info.GetReturnValue().Set(addr);
}

NAN_METHOD(ipremote){
  /* port */
  int port = To<int>(info[1]).FromJust();

  /* deadline */
  int64_t deadline = -1;

  /* set mode default */
  int mode = 1;

  /* ip address */
  String::Utf8Value ip(info[0]);

  /* get an ipaddr */
  ipaddr ipv = ipremote(*ip, port, mode, deadline);
  size_t sz = sizeof(&ipv);

  /* create a node buffer pointer */
  Local<Object> addr = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(addr), &ipv, sz);
  info.GetReturnValue().Set(addr);
}

/******************************************************************************/
/*  TCP library                                                               */
/******************************************************************************/

/* The buffer size is based on typical Ethernet MTU (1500 bytes). Making it
   smaller would yield small suboptimal packets. Making it higher would bring
   no substantial benefit. The value is made smaller to account for IPv4/IPv6
   and TCP headers. Few more bytes are subtracted to account for any possible
   IP or TCP options */
#ifndef TCP_BUFLEN
#define TCP_BUFLEN (1500 - 68)
#endif

enum mill_tcptype {
  MILL_TCPLISTENER,
  MILL_TCPCONN
};

struct mill_tcpsock {
  enum mill_tcptype type;
};

struct mill_tcplistener {
  struct mill_tcpsock sock;
  int fd;
  int port;
};

struct mill_tcpconn {
  struct mill_tcpsock sock;
  int fd;
  size_t ifirst;
  size_t ilen;
  size_t olen;
  char ibuf[TCP_BUFLEN];
  char obuf[TCP_BUFLEN];
  ipaddr addr;
};

typedef struct tcp_s {
  uv_poll_t poll_handle;
  uv_os_sock_t fd;
  Callback *cb;
} tcp_t;

static void tcptune(int s) {
  /* Make the socket non-blocking. */
  int opt = fcntl(s, F_GETFL, 0);
  if (opt == -1)
      opt = 0;
  int rc = fcntl(s, F_SETFL, opt | O_NONBLOCK);
  assert(rc != -1);
  /*  Allow re-using the same local address rapidly. */
  opt = 1;
  rc = setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (opt));
  assert(rc == 0);
  /* If possible, prevent SIGPIPE signal when writing to the connection
      already closed by the peer. */
#ifdef SO_NOSIGPIPE
  opt = 1;
  rc = setsockopt (s, SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof (opt));
  assert (rc == 0 || errno == EINVAL);
#endif
}

static void tcpconn_init(struct mill_tcpconn *conn, int fd) {
  conn->sock.type = MILL_TCPCONN;
  conn->fd = fd;
  conn->ifirst = 0;
  conn->ilen = 0;
  conn->olen = 0;
}

void tcpAccept(uv_poll_t *req, int status, int events) {
  HandleScope scope;
  if (events & UV_READABLE) {
    ipaddr addr;
    socklen_t slen = sizeof(ipaddr);

    tcp_t *ctx;
    ctx = reinterpret_cast<tcp_t *>(req);

    int as = accept(ctx->fd, (struct sockaddr *)&addr, &slen);
    tcptune(as);

    size_t sz = sizeof(struct mill_tcpconn);
    struct mill_tcpconn *conn = (struct mill_tcpconn *)malloc(sz);
    tcpconn_init(conn, as);
    assert(conn);
    conn->addr = addr;

    Local<Value> argv[] = { WrapPointer((tcpsock)conn, sizeof(mill_tcpconn)) };
    ctx->cb->Call(1, argv);
  }
}

NAN_METHOD(tcplisten){
  /* backlog settings */
  int backlog = 10;
  if (info[1]->IsNumber())
    backlog = To<int>(info[1]).FromJust();

  /* dereference and pass ipaddr buffer to tcplisten */
  tcpsock ls = tcplisten(*UnwrapPointer<ipaddr*>(info[0]), backlog);
  assert(ls);
  info.GetReturnValue().Set(WrapPointer(ls, sizeof(&ls)));
}

NAN_METHOD(tcpport){
  tcpsock s = UnwrapPointer<tcpsock>(info[0]);
  int port = tcpport(s);
  info.GetReturnValue().Set(New<Number>(port));
}

NAN_METHOD(tcpaccept){

  tcpsock s = UnwrapPointer<tcpsock>(info[0]);

  /* deadline */
  int64_t deadline = -1;
  if (info[1]->IsNumber())
    deadline = now() + To<int64_t>(info[1]).FromJust();

  if (info[1]->IsFunction()) {
    if(s->type != MILL_TCPLISTENER)
      abort(); // abort trap! trying to pass non-listening socks..
    struct mill_tcplistener *l = (struct mill_tcplistener*)s;

    Callback *cb = new Callback(info[1].As<Function>());
    tcp_t *ctx;
    ctx = reinterpret_cast<tcp_t *>(calloc(1, sizeof(tcp_t)));
    ctx->poll_handle.data = ctx;
    ctx->cb = cb;
    ctx->fd = l->fd;

    uv_poll_init_socket(uv_default_loop(), &ctx->poll_handle, ctx->fd);
    uv_poll_start(&ctx->poll_handle, UV_READABLE, tcpAccept);

    info.GetReturnValue().Set(WrapPointer(ctx, 8));
  } else {
    tcpsock as = tcpaccept(s, deadline);
    assert(as);
    info.GetReturnValue().Set(WrapPointer(as, sizeof(&as)));
  }
}

NAN_METHOD(tcpconnect){
  /* deadline */
  int64_t deadline = -1;
  if (info[1]->IsNumber())
    deadline = now() + To<int64_t>(info[1]).FromJust();

  /* pass an ipremote buffer to tcpconnect */
  tcpsock cs = tcpconnect(*UnwrapPointer<ipaddr*>(info[0]), deadline);
  assert(cs);
  info.GetReturnValue().Set(WrapPointer(cs, sizeof(&cs)));
}

NAN_METHOD(tcpsend){
  /* deadline */
  int64_t deadline = -1;
  if (info[2]->IsNumber())
    deadline = now() + To<int64_t>(info[2]).FromJust();

  size_t sz = tcpsend(UnwrapPointer<tcpsock>(info[0]),
    node::Buffer::Data(info[1]), node::Buffer::Length(info[1]), deadline);

  info.GetReturnValue().Set(New<Number>(sz));
}

NAN_METHOD(tcpflush){
  /* deadline */
  int64_t deadline = -1;
  if (info[1]->IsNumber())
    deadline = now() + To<int64_t>(info[1]).FromJust();

  tcpflush(UnwrapPointer<tcpsock>(info[0]), deadline);
}

NAN_METHOD(tcprecv){
  /* deadline */
  int64_t deadline = -1;
  if (info[2]->IsNumber())
    deadline = now() + To<int64_t>(info[2]).FromJust();

  int rcvbuf = To<int>(info[1]).FromJust();

  char buf[rcvbuf];
  size_t sz = tcprecv(UnwrapPointer<tcpsock>(info[0]), buf, rcvbuf, deadline);

  v8::Local<v8::Object> rc = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(rc), buf, sz);

  info.GetReturnValue().Set(rc);
}

//TODO: delimiters: const char *delims, size_t delimcount
NAN_METHOD(tcprecvuntil){
  /* deadline */
  int64_t deadline = -1;
  if (info[2]->IsNumber())
    deadline = now() + To<int64_t>(info[2]).FromJust();

  /* recv buffer size */
  int rcvbuf = To<int>(info[1]).FromJust();
  char buf[rcvbuf];

  /* recvuntil delimiters */
  tcpsock s = UnwrapPointer<tcpsock>(info[0]);
  size_t sz = tcprecvuntil(s, buf, sizeof(buf), "\r", 1, deadline);

  /* fill recv buffer from OS */
  Local<Value> rc = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(rc), buf, sz);

  info.GetReturnValue().Set(rc);
}

NAN_METHOD(tcpclose){
  tcpclose(UnwrapPointer<tcpsock>(info[0]));
}

NAN_METHOD(tcpattach){
  int fd = To<int>(info[0]).FromJust();
  int listening = To<int>(info[1]).FromJust();

  tcpsock s = tcpattach(fd, listening);

  if (!s)
    perror("attach error");

  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(tcpdetach){
  int fd = tcpdetach(UnwrapPointer<tcpsock>(info[0]));
  assert(fd != -1);
  info.GetReturnValue().Set(New<Number>(fd));
}

/******************************************************************************/
/*  UDP library                                                               */
/******************************************************************************/

NAN_METHOD(udplisten){
  udpsock s = udplisten(*UnwrapPointer<ipaddr*>(info[0]));
  assert(s);
  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(udpport){
  int port = udpport(UnwrapPointer<udpsock>(info[0]));
  info.GetReturnValue().Set(New<Number>(port));
}

NAN_METHOD(udpsend){
  udpsock s = UnwrapPointer<udpsock>(info[0]);
  ipaddr addr = *UnwrapPointer<ipaddr*>(info[1]);
  udpsend(s, addr, node::Buffer::Data(info[2]), node::Buffer::Length(info[2]));
}

NAN_METHOD(udprecv){
  ipaddr addr;
  udpsock s = UnwrapPointer<udpsock>(info[0]);

  char buf[To<int>(info[1]).FromJust()];
  int deadline = now() + To<int>(info[2]).FromJust();

  size_t sz = udprecv(s, &addr, buf, sizeof(buf), deadline);
  Local<Object> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  ipaddrstr(addr, ipstr);

  /*  return new JS object with two properties
   *  • buf: the udp buffer
   *  • addr: a human readable IP address string of the buffer's origin
   */
  Local<Object> obj = New<Object>();
  Set(obj, New("buf").ToLocalChecked(), h);
  Set(obj, New("addr").ToLocalChecked(), New<String>(ipstr).ToLocalChecked());

  info.GetReturnValue().Set(obj);
}

NAN_METHOD(udpclose){
  udpclose(UnwrapPointer<udpsock>(info[0]));
}

NAN_METHOD(udpattach){
  int fd = To<int>(info[0]).FromJust();
  udpsock s = udpattach(fd);
  assert(s);
  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(udpdetach){
  int fd = udpdetach(UnwrapPointer<udpsock>(info[0]));
  assert(fd != -1);
  info.GetReturnValue().Set(New<Number>(fd));
}

/******************************************************************************/
/*  UDP library extensions                                                    */
/******************************************************************************/

NAN_METHOD (sleep) {
  int timeo = To<int>(info[0]).FromJust();
  int ret = reqsleep( timeo );
  info.GetReturnValue().Set(ret);
}

struct mill_udpsock {
  int fd;
  int port;
};

typedef struct udp_s {
  uv_poll_t poll_handle;
  uv_os_sock_t fd;
  Callback *cb;
  int len;
} udp_t;

void udpRead(uv_poll_t *req, int status, int events) {
  HandleScope scope;

  if (events & UV_READABLE) {
    ipaddr addr;
    ssize_t ss;
    socklen_t slen = sizeof(ipaddr);

    udp_t *ctx;
    ctx = reinterpret_cast<udp_t *>(req);

    char buf[ctx->len];
    ss = recvfrom(ctx->fd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &slen);

    if(ss >= 0) {
      ipaddrstr(addr, ipstr);
      Local<Object> o = New<Object>();
      Local<Object> h = NewBuffer(ss).ToLocalChecked();
      memcpy(node::Buffer::Data(h), buf, ss);

      Set(o, New("buf").ToLocalChecked(), h);
      Set(o, New("addr").ToLocalChecked(), New<String>(ipstr).ToLocalChecked());

      Local<Value> argv[] = { o };
      ctx->cb->Call(1, argv);
    }
  }
}

NAN_METHOD(udprecva){
  Callback *cb = new Callback(info[2].As<Function>());
  udpsock s = UnwrapPointer<udpsock>(info[0]);

  udp_t *context;
  context = reinterpret_cast<udp_t *>(calloc(1, sizeof(udp_t)));
  context->poll_handle.data = context;
  context->cb = cb;
  context->fd = s->fd;
  context->len = To<int>(info[1]).FromJust();

  if (context->fd != 0) {
    uv_poll_init_socket(uv_default_loop(), &context->poll_handle, context->fd);
    uv_poll_start(&context->poll_handle, UV_READABLE, udpRead);
    info.GetReturnValue().Set(WrapPointer(context, 8));
  }
}

/******************************************************************************/
/*  UNIX library                                                              */
/******************************************************************************/

NAN_METHOD(unixlisten){
  String::Utf8Value sockname(info[0]);
  char *name = *sockname;
  struct stat st;
  if (stat(name, &st) == 0)
    assert(unlink(name) == 0);

  unixsock ls = unixlisten(name, 10);
  assert(ls);

  info.GetReturnValue().Set(WrapPointer(ls, sizeof(&ls)));
}

//TODO: deadline
NAN_METHOD(unixaccept){
  unixsock as = unixaccept(UnwrapPointer<unixsock>(info[0]), -1);
  assert(as);

  info.GetReturnValue().Set(WrapPointer(as, sizeof(&as)));
}

NAN_METHOD(unixconnect){
  String::Utf8Value sockname(info[0]);
  char *name = *sockname;
  unixsock cs = unixconnect(name);
  assert(cs);

  info.GetReturnValue().Set(WrapPointer(cs, sizeof(&cs)));
}

NAN_METHOD(unixpair){
  unixsock a = UnwrapPointer<unixsock>(info[0]);
  unixsock b = UnwrapPointer<unixsock>(info[1]);

  /* socketpair(AF_UNIX, SOCK_STREAM, 0, fd); */
  unixpair(&a, &b);
  assert(errno == 0);
}

//TODO: deadline
NAN_METHOD(unixsend){
  size_t sz = unixsend(UnwrapPointer<unixsock>(info[0]),
    node::Buffer::Data(info[1]), node::Buffer::Length(info[1]), -1);

  info.GetReturnValue().Set(New<Number>(sz));
}

//TODO: deadline
NAN_METHOD(unixflush){
  unixflush(UnwrapPointer<unixsock>(info[0]), -1);
}

//TODO: deadline
NAN_METHOD(unixrecv){
  int rcvbuf = To<int>(info[1]).FromJust();

  /* should make it a static char */
  char buf[rcvbuf];
  size_t sz = unixrecv(UnwrapPointer<unixsock>(info[0]), buf, rcvbuf, -1);

  v8::Local<v8::Object> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
}

//TODO: deadline
NAN_METHOD(unixrecvuntil){
  int rcvbuf = To<int>(info[1]).FromJust();
  unixsock s = UnwrapPointer<unixsock>(info[0]);

  char buf[rcvbuf];
  size_t sz = unixrecvuntil(s, buf, rcvbuf, "\r", 1, -1);

  Local<Value> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
}

NAN_METHOD(unixclose){
  unixclose(UnwrapPointer<unixsock>(info[0]));
}

NAN_METHOD(unixattach){
  int fd = To<int>(info[0]).FromJust();
  int listening = To<int>(info[1]).FromJust();

  unixsock s = unixattach(fd, listening);
  assert(s);

  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(unixdetach){
  int fd = unixdetach(UnwrapPointer<unixsock>(info[0]));
  assert(fd != -1);
  info.GetReturnValue().Set(New<Number>(fd));
}

NAN_METHOD(goredump){ goredump(); };
NAN_METHOD(gotrace) { gotrace(1); };

/* basic test to verify build */
void worker(int count, const char *text) {
    for(int i = 0; i != count; ++i) {
        printf("%s\n", text);
        msleep(10);
    }
}
NAN_METHOD(test){
  go(worker(4, "a"));
  go(worker(2, "b"));
  go(worker(3, "c"));
  msleep(100); return;
}

#define EXPORT_METHOD(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(),                                   \
    Nan::GetFunction(New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  HandleScope scope;

  /* ip resolution */
  EXPORT_METHOD(target, iplocal);
  EXPORT_METHOD(target, ipremote);

  /* tcp library */
  EXPORT_METHOD(target, tcplisten);
  EXPORT_METHOD(target, tcpaccept);
  EXPORT_METHOD(target, tcpconnect);
  EXPORT_METHOD(target, tcpsend);
  EXPORT_METHOD(target, tcpflush);
  EXPORT_METHOD(target, tcprecv);
  EXPORT_METHOD(target, tcprecvuntil);
  EXPORT_METHOD(target, tcpattach);
  EXPORT_METHOD(target, tcpdetach);
  EXPORT_METHOD(target, tcpport);
  EXPORT_METHOD(target, tcpclose);

  /* udp library */
  EXPORT_METHOD(target, udplisten);
  EXPORT_METHOD(target, udpport);
  EXPORT_METHOD(target, udpsend);
  EXPORT_METHOD(target, udprecv);
  EXPORT_METHOD(target, udpclose);
  EXPORT_METHOD(target, udpattach);
  EXPORT_METHOD(target, udpdetach);

  /* extensions */
  EXPORT_METHOD(target, sleep);
  EXPORT_METHOD(target, udprecva);

  /* unix library */
  EXPORT_METHOD(target, unixlisten);
  EXPORT_METHOD(target, unixaccept);
  EXPORT_METHOD(target, unixconnect);
  EXPORT_METHOD(target, unixpair);
  EXPORT_METHOD(target, unixsend);
  EXPORT_METHOD(target, unixflush);
  EXPORT_METHOD(target, unixrecv);
  EXPORT_METHOD(target, unixrecvuntil);
  EXPORT_METHOD(target, unixclose);
  EXPORT_METHOD(target, unixattach);
  EXPORT_METHOD(target, unixdetach);

  /* debug */
  EXPORT_METHOD(target, gotrace);
  EXPORT_METHOD(target, goredump);
  EXPORT_METHOD(target, test);
}

NODE_MODULE(mill, Init)
