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
var mill = require('libmill');
var ls = mill.tcplisten(5555);
console.log('listening on port %s', mill.tcpport(ls));

while(1){
  var as = mill.tcpaccept(ls);
  console.log('\nnew connection\nmsg recvd: ' + mill.tcprecvuntil(as));
  mill.tcpclose(as);
}
```
### `tcpconnect()`
```js
// connect socket
var cs = require('libmill').tcpconnect('127.0.0.1', 5555);
var str = 'go style concurrency for node', num = 1;

// minimize # of calls to OS kernel
// send a few msg before tcpflush
send('msg # ', str);
send('msg # ', str);

// msg delimiter is set to '\r', tells tcprecvuntil when to finish
send('msg #', str+'\r');
function send(n, msg){
  n+=num+++msg+'\n';
  require('libmill').tcpsend(cs, new Buffer(n));
}

// tcpsend() stores data in user space and tcpflush() pushes it to kernel
require('libmill').tcpflush(cs);
```
# udp library
### `udplisten()` and `udprecv()`
```js
// udp recv
var m = require('libmill');
var s = m.udplisten(5555);

process.stdout.write('udp socket listening on port: '
  + m.udpport(s) + '\n');

while(1)
  process.stdout.write(m.udprecv(s, 13) + '\n');

```
### `udpsend()`
```js
var mill = require('libmill');
var s = mill.udplisten(5555);
mill.udpsend(s, '10.0.1.13', 5555, new Buffer('Hello, world!'));
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
