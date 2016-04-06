# node.js function access to libmill
[![Build Status](https://circleci.com/gh/reqshark/mill/tree/master.svg?style=svg)](https://circleci.com/gh/reqshark/mill/tree/master)

go style concurrency for node.

an experiment

```bash
$ npm i libmill

# or
$ git clone https://github.com/reqshark/mill.git && cd mill
$ npm i && npm t

# or start over with:
$ make clean
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

/* next, to get udp msgs, use udprecv
   if you pass a cb function as the 3rd param, the call will use libuv async
   the callback's param will contain your msg

   otherwise w/out a cb, it will block and udprecv's return value is your msg */


/* the non-blocking way (a for async) */
lib.udprecv(ls, 255, function (msg) {
  var buf = String(msg.buf) /* msg.buf is a node buffer of the packet body */
  var addr = msg.addr  /* string address of packet origin */
});

/* the blocking way  */
while (1) {
  var sz = 13;
  var deadline = 10;

  var msg = lib.udprecv(ls, sz, deadline); /* deadline is optional param */
  process.stdout.write(msg.buf + '\n');
}
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
