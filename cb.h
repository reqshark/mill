//coroutine void callback(Local<Function> cb) {
//  Local<Value> argv[] = { New("hello world").ToLocalChecked() };
//  cb->Call(1, argv);
//}

NAN_METHOD(cbStyleA) {
  Local<Function> cb = info[0].As<Function>();
  const unsigned argc = 1;
  Local<Value> argv[argc] = { New("A").ToLocalChecked() };
  MakeCallback(GetCurrentContext()->Global(), cb, argc, argv);
}

NAN_METHOD(cbStyleB) {
  Callback *cb = new Callback(info[0].As<Function>());
  Local<Value> argv[] = { New("B").ToLocalChecked() };
  cb->Call(1, argv);
}

NAN_METHOD(cbStyleC) {
  Local<Function> cb = info[0].As<Function>();
  Local<Value> argv[] = { New("C").ToLocalChecked() };
  Nan::Call(cb, New<Object>(), 1, argv);
}
