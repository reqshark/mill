# nmsg

[![Build Status](https://travis-ci.org/reqshark/nmsg.svg?branch=master)](https://travis-ci.org/reqshark/nmsg) &nbsp;&nbsp;&nbsp;&nbsp; [![npmbadge](https://nodei.co/npm/nmsg.png?mini=true)](https://www.npmjs.com/package/nmsg)

#### nanomsg utility for `iojs`

# use
```js
var nmsg = require('nmsg')
var fam = nmsg.AF_SP
var pub = nmsg.socket(fam, nmsg.NN_PUB)
var sub = nmsg.socket(fam, nmsg.NN_SUB)

var addr = 'tcp://127.0.0.1:55555'
nmsg.bind(pub, addr)
nmsg.connect(sub, addr)

setTimeout(function(){
  nmsg.send(pub, 'Hello from nanomsg!')
  console.log('received: ' + nmsg.recv(sub))
},10)
```

# test
```bash
$ make clean && make && make check
```
