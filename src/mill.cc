#include "nan.h"
#include <stdio.h>

extern "C" {
#include "patch.h"
}

//void worker(int count, const char *text) {
//    int i;
//    for(i = 0; i != count; ++i) {
//        printf("%s\n", text);
//        msleep(10);
//    }
//}

#if (NODE_MODULE_VERSION < 10)
#define RUNLOOP_SEMANTICS ev_run(ev_default_loop(), EVRUN_ONCE)
#else
#define RUNLOOP_SEMANTICS uv_run(uv_default_loop(), UV_RUN_ONCE)
#endif

using v8::Handle;
using v8::Object;
using v8::FunctionTemplate;

#define EXPORT_METHOD(C, S) C->Set(NanNew(# S), NanNew<FunctionTemplate>(S)->GetFunction());

NAN_METHOD(stall) {
  NanScope();
  RUNLOOP_SEMANTICS;
  NanReturnUndefined();
}


void Init(Handle<Object> exports) {
  NanScope();
  EXPORT_METHOD(exports, stall);
}

NODE_MODULE(stall, Init)
