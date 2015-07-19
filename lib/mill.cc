#include "nan.h"

extern "C" {
#include "patch.h"
}

using v8::Local;
using v8::Object;
using v8::FunctionTemplate;

#define EXPORT_METHOD(C, S) C->Set(NanNew(# S), NanNew<FunctionTemplate>(S)->GetFunction());

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
  NanReturnUndefined();
}

NAN_METHOD(trace){
  gotrace(1);
  NanReturnUndefined();
};

void Init(Local<Object> e) {
  NanScope();
  EXPORT_METHOD(e, test);
  EXPORT_METHOD(e, trace);
}

NODE_MODULE(mill, Init)
