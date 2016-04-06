#define MAX_INPUT_LEN 4096

size_t reinterpret_msg (const char *input, char *cast) {
  size_t len = strlen(input);
  memcpy(cast, input, len);
  return len;
}

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
  ret(New(n).ToLocalChecked());
}

/* return the crypto_box_primitive */
NAN_METHOD(box_primitive){
  const char *box_primitive = crypto_box_primitive();

  ret(New( box_primitive ).ToLocalChecked());
}

NAN_METHOD(setkeys){

}
