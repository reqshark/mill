# node.js function access to libmill
[![Build Status](https://travis-ci.org/reqshark/mill.svg?branch=master)](https://travis-ci.org/reqshark/mill)

go style concurrency for node.

an experiment

```bash
$ npm i libmill

# or
$ git clone https://github.com/reqshark/mill.git && cd mill
$ git submodule update --init
$ npm i && npm t

# or build with:
$ make
```

# tcp library

for msgs over tcp/ip, a server can use `tcplisten()`. once a client connects, the server can use `tcpaccept()` to establish bidirectional tcp streams.

### `tcplisten()` and `tcpaccept()`
```js
var m = require('..');

var ls = m.tcplisten(5555);
console.log('listening on port %s', m.tcpport(ls));

while(1){
  var as = m.tcpaccept(ls);
  console.log('\nnew connection\nmsg recvd: ' + m.tcprecvuntil(as));
  m.tcpclose(as);
}
```
### `tcpconnect()`
```js
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
```
# test
```bash
$ make clean && make && make check

# or
$ rm -rf node_modules build
$ npm i
$ npm t
```

## license

MIT

<sub>*tested on travis linux and osx.*</sub>
