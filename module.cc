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

NAN_METHOD(tcplisten){
  int port = To<int>(info[0]).FromJust();

  ipaddr addr = iplocal(NULL, port, 0);
  tcpsock ls = tcplisten(addr, -1);

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

NAN_METHOD(tcprecv){
  int rcvbuf = To<int>(info[1]).FromJust();

  char buf[rcvbuf];
  size_t sz = tcprecv(UnwrapPointer<tcpsock>(info[0]), buf, rcvbuf, -1);

  buf[sz - 1] = 0;

  v8::Local<v8::Object> h = NewBuffer(sz).ToLocalChecked();
  memcpy(node::Buffer::Data(h), buf, sz);

  info.GetReturnValue().Set(h);
}

NAN_METHOD(tcprecvuntil){
  tcpsock s = UnwrapPointer<tcpsock>(info[0]);

  char buf[256];
  size_t sz = tcprecvuntil(s, buf, sizeof(buf), "\r", 1, -1);

  buf[sz - 1] = 0;

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

NAN_METHOD(tcpflush){ tcpflush(UnwrapPointer<tcpsock>(info[0]), -1); }
NAN_METHOD(tcpclose){ tcpclose(UnwrapPointer<tcpsock>(info[0])); }
NAN_METHOD(trace){ gotrace(1);};

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

  /* debug */
  EXPORT_METHOD(target, test);
  EXPORT_METHOD(target, trace);
}

NODE_MODULE(mill, Init)
