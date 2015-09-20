var m = require('..');

var ls = m.tcplisten(5555);
console.log('listening on port %s', m.tcpport(ls));

while(1){
  var as = m.tcpaccept(ls);
  console.log('\nnew connection\nmsg recvd: ' + m.tcprecvuntil(as));
  m.tcpclose(as);
}
