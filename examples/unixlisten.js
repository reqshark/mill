var m = require('..');
var ls = m.unixlisten('helloworld');
while(1){
  var as = m.unixaccept(ls);
  m.unixsend(as, new Buffer('some low latency msgs!\n'));
  m.unixflush(as);
  process.stdout.write('\nnew connection\nmsg recvd: '
    + m.unixrecvuntil(as) + '\n');
  m.unixclose(as);
}
