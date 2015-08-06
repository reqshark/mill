
// step1
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

// step2
NAN_METHOD(s2){
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
    tcpsend(as, "What's your name?\r\n", 19, -1);
    tcpflush(as, -1);

    char inbuf[256];
    size_t sz = tcprecvuntil(as, inbuf, sizeof(inbuf), "\r\n", 2, -1);

    inbuf[sz - 1] = 0;
    char outbuf[256];
    int rc = snprintf(outbuf, sizeof(outbuf), "Hello, %s!\r\n", inbuf);

    sz = tcpsend(as, outbuf, rc, -1);
    tcpflush(as, -1);

    tcpclose(as);
  }
}

// step3
NAN_METHOD(s3){
  NanScope();
  int n = args[0].number;
  ipaddr addr = iplocal(NULL, args[2].number, 0);
  ls[n] = tcplisten(addr);

  if (!ls[n]) {
    perror("Can't open listening socket");
    ret(NanNew<Number>(n));;
  }

  while(1) {
    int rc;
    size_t sz;
    
    tcpsock as = tcpaccept(ls[n], -1);
    if (!as) continue;

    tcpsend(as, "What's your name?\r\n", 19, -1);
    if (errno != 0) goto cleanup;

    tcpflush(as, -1);
    if (errno != 0) goto cleanup;

    char inbuf[256];
    sz = tcprecvuntil(as, inbuf, sizeof(inbuf), "\r", 1, -1);
    if (errno != 0) goto cleanup;

    inbuf[sz - 1] = 0;
    char outbuf[256];
    rc = snprintf(outbuf, sizeof(outbuf), "Hello, %s!\r\n", inbuf);

    sz = tcpsend(as, outbuf, rc, -1);
    if (errno != 0) goto cleanup;
    tcpflush(as, -1);
    if (errno != 0) goto cleanup;

    cleanup:
      tcpclose(as);
  }
}
