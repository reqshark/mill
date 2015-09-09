#include "nan.h"

extern "C" {
#include "libmill.h"
}

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

#define EXPORT_METHOD(C, S)                                                    \
  Nan::Set(C, Nan::New(#S).ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  Nan::HandleScope scope;
  EXPORT_METHOD(target, test);
  EXPORT_METHOD(target, trace);
}
NODE_MODULE(mill, Init)
