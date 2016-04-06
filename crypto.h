#define MAX_INPUT_LEN 4096


static unsigned char *nonce[crypto_box_NONCEBYTES];
static char nhex[crypto_box_NONCEBYTES * 2 + 1];

/* nbuf uses a pointer kept back in the binding */
static char *nbuf() {
  size_t sz = crypto_box_NONCEBYTES;
  randombytes_buf(&nonce, sz);
  sodium_bin2hex(nhex, sz * 2 + 1, (unsigned char *)nonce, sz);
  return nhex;
}

/* return a nonce hex to node */
NAN_METHOD(nstr){
  char *n = nbuf();
  info.GetReturnValue().Set(New(n).ToLocalChecked());
}
