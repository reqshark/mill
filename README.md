# ADVISORY NOTICE: dont use this, I just unpublished it on npm

use the [official binding](https://github.com/nickdesaulniers/node-nanomsg) or the new [iojs-nanomsg](https://www.npmjs.com/package/iojs-nanomsg) package i'm working on. 

all of the code here was just my starting point. false: my true starting point was learning from Fatih Kaya's code, [i forked his nanomsg binding experiments](https://github.com/reqshark/nanomsg.node) and.. well anyway at some point during January 19th, 2015 I ported all the code in this repo over to the [iojs-nanomsg project](https://github.com/reqshark/nanomsg.iojs), from there I added node streams and a lot of general improvements to how the binding handles buffer objects getting coverted back and forth between node and the c library. 

anyway i'm building out more of that stuff right now on github here: https://github.com/reqshark/nanomsg.iojs also if you have any ideas please submit PRs and Fork it or submit issues there, thanks. -reqshark

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
[![philosophy](benchmark/ff.png)](http://en.wikipedia.org/wiki/Unix_philosophy#.22Worse_is_better.22)
```bash
$ make clean && make && make check
```
<sub>*binding node v0.8 and up*</sub>
