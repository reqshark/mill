#include "nan.h"

extern "C" {
#include "libmill.h"
}

using v8::String;
using v8::Local;
using v8::Object;
using v8::Number;
using v8::FunctionTemplate;

//using Nan::Callback;
//using Nan::New;
//#include "tutorial.cc"

void worker(int count, const char *text) {
    int i;
    for(i = 0; i != count; ++i) {
        printf("%s\n", text);
        msleep(10);
    }
}

NAN_METHOD(test){
  go(worker(4, "a"));
  go(worker(2, "b"));
  go(worker(3, "c"));
  msleep(100);
  return;
}

NAN_METHOD(trace){
  gotrace(1);
  return;
};

static tcpsock cs[1024];
NAN_METHOD(connect){
  String::Utf8Value addrstr(info[0]);
  int port = Nan::To<int>(info[1]).FromJust();
  int n = Nan::To<int>(info[2]).FromJust();

  ipaddr addr = ipremote(*addrstr, port, 0, -1);
  cs[n] = tcpconnect(addr, -1);
  info.GetReturnValue().Set(Nan::New<Number>(n));
}

//static tcpsock ls[1024];
//NAN_METHOD(listen){
//  int n = Nan::To<int>(info[0]).FromJust();
//  String::Utf8Value addr(info[1]);
//}


#define EXPORT_METHOD(C, S)                                                    \
  Nan::Set(C, Nan::New(#S).ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  Nan::HandleScope scope;
//  EXPORT_METHOD(target, listen);
  EXPORT_METHOD(target, connect);
  EXPORT_METHOD(target, test);
  EXPORT_METHOD(target, trace);
}
NODE_MODULE(mill, Init)
