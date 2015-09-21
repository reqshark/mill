#include "nan.h"

using v8::String;
using v8::Local;
using v8::Object;
using v8::Number;
using v8::FunctionTemplate;
using v8::Value;

using Nan::To;
using Nan::NewBuffer;

#include "ref.h"

extern "C" {
#include "libmill.h"
}

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

NAN_METHOD(tcplisten){
  int port = To<int>(info[0]).FromJust();

  ipaddr addr = iplocal(NULL, port, 0);
  tcpsock ls = tcplisten(addr, 10);

  if (!ls)
    perror("Can't open listening socket");

  info.GetReturnValue().Set(WrapPointer(ls, sizeof(&ls)));
}

NAN_METHOD(tcpaccept){
  tcpsock as = tcpaccept(UnwrapPointer<tcpsock>(info[0]), -1);

  if (!as)
    perror("accept error");

  info.GetReturnValue().Set(WrapPointer(as, sizeof(&as)));
}

NAN_METHOD(tcpconnect){
  int port = To<int>(info[1]).FromJust();
  String::Utf8Value ip(info[0]);

  ipaddr addr = ipremote(*ip, port, 0, -1);
  tcpsock cs = tcpconnect(addr, -1);

  if (!cs)
    perror("connect error");

  info.GetReturnValue().Set(WrapPointer(cs, sizeof(&cs)));
}

NAN_METHOD(tcpsend){
  size_t len = tcpsend(UnwrapPointer<tcpsock>(info[0]),
    node::Buffer::Data(info[1]), node::Buffer::Length(info[1]), -1);

  info.GetReturnValue().Set(Nan::New<Number>(len));
}

//TODO: deadline
NAN_METHOD(tcprecv){
  int rcvbuf = To<int>(info[1]).FromJust();

  char buf[rcvbuf];
  size_t sz = tcprecv(UnwrapPointer<tcpsock>(info[0]), buf, rcvbuf, -1);

  v8::Local<v8::Object> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
}

//TODO: deadline
NAN_METHOD(tcprecvuntil){
  tcpsock s = UnwrapPointer<tcpsock>(info[0]);

  char buf[256];
  size_t sz = tcprecvuntil(s, buf, sizeof(buf), "\r", 1, -1);

  Local<Value> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
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
  info.GetReturnValue().Set(Nan::New<Number>(fd));
}

NAN_METHOD(tcpport){
  tcpsock s = UnwrapPointer<tcpsock>(info[0]);
  int port = tcpport(s);
  info.GetReturnValue().Set(Nan::New<Number>(port));
}

//TODO: deadline
NAN_METHOD(tcpflush){
  tcpflush(UnwrapPointer<tcpsock>(info[0]), -1);
}

NAN_METHOD(tcpclose){
  tcpclose(UnwrapPointer<tcpsock>(info[0]));
}

NAN_METHOD(udplisten){
  int port = To<int>(info[0]).FromJust();

  ipaddr addr = iplocal(NULL, port, 0);
  udpsock s = udplisten(addr);

  if (!s)
    perror("udp socket listen err");

  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(udpport){
  int port = udpport(UnwrapPointer<udpsock>(info[0]));
  info.GetReturnValue().Set(Nan::New<Number>(port));
}

NAN_METHOD(udpsend){
  String::Utf8Value ip(info[1]);
  int port = To<int>(info[2]).FromJust();

  ipaddr addr = ipremote(*ip, port, 0, -1);
  udpsock s = UnwrapPointer<udpsock>(info[0]);
  udpsend(s, addr, node::Buffer::Data(info[3]), node::Buffer::Length(info[3]));
}

NAN_METHOD(udprecv){
  int rcvbuf = To<int>(info[1]).FromJust();
  udpsock s = UnwrapPointer<udpsock>(info[0]);

  char buf[rcvbuf];
  ipaddr addr;
  size_t sz = udprecv(s, &addr, buf, sizeof(buf), -1);

  v8::Local<v8::Object> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
}

NAN_METHOD(udpclose){
  udpclose(UnwrapPointer<udpsock>(info[0]));
}

NAN_METHOD(udpattach){
  int fd = To<int>(info[0]).FromJust();
  udpsock s = udpattach(fd);

  if (!s)
    perror("udp fd attach error");

  info.GetReturnValue().Set(WrapPointer(s, sizeof(&s)));
}

NAN_METHOD(udpdetach){
  int fd = udpdetach(UnwrapPointer<udpsock>(info[0]));
  assert(fd != -1);
  info.GetReturnValue().Set(Nan::New<Number>(fd));
}

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

  info.GetReturnValue().Set(Nan::New<Number>(sz));
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
  info.GetReturnValue().Set(Nan::New<Number>(fd));
}

NAN_METHOD(goredump){ goredump();};
NAN_METHOD(gotrace){ gotrace(1);};

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
  Nan::Set(C, Nan::New(#S).ToLocalChecked(),                                   \
    Nan::GetFunction(Nan::New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  Nan::HandleScope scope;

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
