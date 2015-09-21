var m = require('..')
var str = ' go style concurrency for node', num = 1;

var cs = m.unixconnect('helloworld');
process.stdout.write('client recv: ' + m.unixrecv(cs, 23) + '\n');

/* msg delimiter is set to '\r', tells unixrecvuntil when to finish */
send('msg # ', str);
send('msg # ', str);
send('msg #', str+'\r');

function send(n, msg){
  n+=num+++msg+'\n';
  m.unixsend(cs, new Buffer(n));
}

m.unixflush(cs);
