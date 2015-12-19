# node.js function access to libmill
[![Build Status](https://travis-ci.org/reqshark/mill.svg?branch=master)](https://travis-ci.org/reqshark/mill)

go style concurrency for node.

### 0.0.6 is broken on linux at the moment, use 0.0.5 or wait for 0.0.7

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
var lib = require('libmill');

/* create an ipaddr */
var ipaddr = lib.iplocal(5555);

/* listen on the ipaddr */
var ls = lib.tcplisten(ipaddr);
console.log('listening on port %s', lib.tcpport(ls));

while(1){
  /* accept and establish tcp connection */
  var as = lib.tcpaccept(ls);
  console.log('\nnew connection\nmsg recvd: ' + lib.tcprecvuntil(as));

  /* close tcp connection */
  lib.tcpclose(as);
}
```
### `tcpconnect()`
```js
var lib = require('libmill');

/* create an ipaddr */
var ipaddr = lib.iplocal(5555);

/* connect over the ipaddr */
var cs = lib.tcpconnect(ipaddr);
var str = 'go style concurrency for node', num = 1;

/* send a few msg before tcpflush */
/* delimiter is set to '\r', tells tcprecvuntil when to finish */
send('msg # ', str);
send('msg # ', str);
send('msg #', str+'\r');
function send(n, msg){
  n+= num+++msg+'\n';
  lib.tcpsend(cs, new Buffer(n));
}

/* tcpsend() stores data in user space and tcpflush() pushes it to kernel */
lib.tcpflush(cs);
```

# udp library
### `udplisten()` and `udprecv()`
```js
var lib = require('libmill');

/* create an ipaddr */
var ipaddr = lib.iplocal(4444);

/* listen on ipaddr */
var ls = lib.udplisten(ipaddr);
process.stdout.write('udp socket listening on port: ' + lib.udpport(ls) + '\n');

while(1)
  process.stdout.write(lib.udprecv(ls, 13) + '\n');
```

### `udpsend()`
```js
var s = lib.udplisten(ipaddr);
var buf = new Buffer('Hello, world!');

lib.udpsend(s, ipaddr, buf);
```
# test
see [`test` directory](test)

## license

MIT

<sub>*tested on travis linux and osx.*</sub>
