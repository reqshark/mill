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
  NanScope();
  go(worker(4, "a"));
  go(worker(2, "b"));
  go(worker(3, "c"));
  msleep(100);
  NanReturnUndefined();
}

void Init(Local<Object> exports) {
  NanScope();
  EXPORT_METHOD(exports, test);
}

NODE_MODULE(mill, Init)
