#include "node.h"
#include "node_buffer.h"
#include "nan.h"
#define EXPORT_METHOD(C, S) C->Set(NanNew(# S), NanNew<FunctionTemplate>(S)->GetFunction());

#if (NODE_MODULE_VERSION < 10)
#define RUNLOOP_SEMANTICS ev_run(ev_default_loop(), EVRUN_ONCE)
#else
#define RUNLOOP_SEMANTICS uv_run(uv_default_loop(), UV_RUN_ONCE)
#endif

using v8::Handle;
using v8::Object;
using v8::FunctionTemplate;

NAN_METHOD(stall) {
  NanScope();
  RUNLOOP_SEMANTICS;
  NanReturnUndefined();
}

void Init(Handle<Object> exports) {
  NanScope();
  EXPORT_METHOD(exports, stall);
}

NODE_MODULE(wait, Init)
