// connect socket
var cs = require('..').tcpconnect('127.0.0.1', 5555);
var str = ': go style concurrency for node', num = 1;

// minimize # of calls to OS kernel
// send a few msg before tcpflush
send('msg #', str);
send('msg #', str);

// msg delimiter is set to '\r', tells tcprecvuntil when to finish
send('msg #', str+'\r');
function send(n, msg){
  n+=num+++msg+'\n';
  require('..').tcpsend(cs, new Buffer(n));
}

// tcpsend() stores data in user space and tcpflush() pushes it to kernel
require('..').tcpflush(cs);
