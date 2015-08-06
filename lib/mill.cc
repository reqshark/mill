#include "nan.h"

extern "C" {
#include "include/libmill.h"
}

using v8::Local;
using v8::Object;
using v8::Number;
using v8::FunctionTemplate;

#define ret NanReturnValue
#define utf8 v8::String::Utf8Value
#define number As<Number>()->IntegerValue()
#define T(C,S)C->Set(NanNew(# S),NanNew<FunctionTemplate>(S)->GetFunction());

static tcpsock ls[1024];

NAN_METHOD(s1){
  NanScope();
  int n = args[0].number;
  ipaddr addr = iplocal(NULL, args[2].number, 0);
  ls[n] = tcplisten(addr);
  if(!ls[n]) {
    perror("Can't open listening socket");
    ret(NanNew<Number>(n));;
  }
  while(1) {
    tcpsock as = tcpaccept(ls[n], -1);
    printf("New connection!\n");
    tcpclose(as);
  }
}

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
  T(e, s1);
  T(e, test);
  T(e, trace);
}

NODE_MODULE(mill, Init)
