var m = require('..');

var ls = m.tcplisten(5555);
console.log('listening on port %s', m.tcpport(ls));

while(1){
  var as = m.tcpaccept(ls);

  m.tcpsend(as, new Buffer('some low latency msgs!\n'));
  m.tcpflush(as);

  process.stdout.write('\nnew connection\nmsg recvd: '
    + m.tcprecvuntil(as) + '\n');

  m.tcpclose(as);
}
