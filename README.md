# nmsg

[![Build Status](https://travis-ci.org/reqshark/nmsg.svg?branch=master)](https://travis-ci.org/reqshark/nmsg) &nbsp;&nbsp;&nbsp;&nbsp; [![npmbadge](https://nodei.co/npm/nmsg.png?mini=true)](https://www.npmjs.com/package/nmsg)

#### [nanomsg](https://github.com/nanomsg/nanomsg) utility for `iojs`

# use
```js
var nmsg = require('nmsg')
var pub = nmsg.socket('pub')
pub.bind('tcp://127.0.0.1:55555')

//set exact semantics of the socket from a configurable param
var fam = 'af'
var sub = nmsg.socket('sub', fam)
sub.connect('tcp://127.0.0.1:55555')

setTimeout(function(){
  pub.send('Hello from nanomsg!')
  console.log('received:', sub.recv())
},10)
```

# test
on node v0.8 and up

```bash
$ make clean && make && make check
```
