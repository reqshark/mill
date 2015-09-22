var str = ' go style concurrency for node\n';
var stdo = process.stdout;
var lib = require('..');
var num = 1;

/* connect tcp socket */
var ipaddr = lib.iplocal(44444);
var cs = lib.tcpconnect(ipaddr);

/* setting exact rcvbuf size to 23 */
var recv = lib.tcprecv(cs, 23);

/* log inbound msg to standard out */
stdo.write('client recv: ' + recv + '\n');

/* msg delimiter is set to '\r', tells tcprecvuntil when to finish */
send('msg # ', str);
send('msg # ', str);
send('msg #', str+'\r');
function send(n, msg){
  n+=num+++msg;
  lib.tcpsend(cs, new Buffer(n));
}


/*
 * minimize # of calls to OS kernel:
 * tcpsend() stores data in user space
 * tcpflush() pushes it to kernel
 */

lib.tcpflush(cs);
