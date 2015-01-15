
NAN_METHOD(Socket) {
  NanScope();
  int64_t type = args[1].As<Number>()->IntegerValue();
  int ret = nn_socket(args[0].As<Number>()->IntegerValue(), type);
  if(type == NN_SUB)
    nn_setsockopt (ret, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(Close){
  NanScope();
  int ret = nn_close(args[0].As<Number>()->IntegerValue());
  NanReturnValue(NanNew<Number>(ret));
}

NAN_METHOD(Bind) {
  NanScope();
  int64_t s = args[0].As<Number>()->IntegerValue();
  String::Utf8Value addr(args[1]);
  NanReturnValue(NanNew<Number>(nn_bind(s, *addr)));
}

NAN_METHOD(Connect) {
  NanScope();
  int64_t s = args[0].As<Number>()->IntegerValue();
  String::Utf8Value addr(args[1]);
  NanReturnValue(NanNew<Number>(nn_connect(s, *addr)));
}

NAN_METHOD(Send) {
  NanScope();
  Local<Object> obj = args[1]->ToObject();
  nn_send (args[0].As<Number>()->IntegerValue(), node::Buffer::Data(obj),
    node::Buffer::Length(obj), args[2].As<Number>()->IntegerValue());
  NanReturnUndefined();
}

NAN_METHOD(Recv) {
  NanScope();
  char* buf = NULL;
  nn_recv (args[0].As<Number>()->IntegerValue(),
    &buf, NN_MSG, args[1].As<Number>()->IntegerValue());

  //this part is odd
  nn_freemsg (buf);
  NanReturnValue(NanNew<String>(buf));
}
