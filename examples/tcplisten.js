var logstr = '\nnew connection\nmsg recvd: ';
var stdo = process.stdout;
var lib = require('..');

/* start listening on an ipaddr */
var ls = lib.tcplisten( lib.iplocal(44444) );
stdo.write('listening on port ' + lib.tcpport(ls) +'\n');

while(1){
  /* accept and establish tcp connection */
  var as = lib.tcpaccept(ls);

  /* send and flush a msg */
  lib.tcpsend(as, new Buffer('some low latency msgs!\n'));
  lib.tcpflush(as);

  /* recv and log inbound msg to standard out */
  var recv = lib.tcprecvuntil(as);
  stdo.write(logstr + recv + '\n');

  /* close the connection */
  lib.tcpclose(as);
}
