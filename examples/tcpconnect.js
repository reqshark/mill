// connect socket
var m = require('..')
var cs = m.tcpconnect('10.0.1.13', 5555);
var str = ' go style concurrency for node', num = 1;

process.stdout.write('client recv: '
  /* setting the exact rcvbuf size to 23 */
  + m.tcprecv(cs, 23) + '\n');

/* minimize # of calls to OS kernel */
send('msg # ', str);
send('msg # ', str);

/* msg delimiter is set to '\r', tells tcprecvuntil when to finish */
send('msg #', str+'\r');
function send(n, msg){
  n+=num+++msg+'\n';
  m.tcpsend(cs, new Buffer(n));
}

/* tcpsend() stores data in user space and tcpflush() pushes it to kernel */
m.tcpflush(cs);
