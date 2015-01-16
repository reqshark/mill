var profiler = require('v8-profiler');
profiler.startProfiling('nano')
var nmsg = require('../')
var pub = nmsg.socket('pub')
pub.bind('tcp://127.0.0.1:55555')
var sub = nmsg.socket('sub')
sub.connect('tcp://127.0.0.1:55555')
setTimeout(function(){
  pub.send('Hello from nanomsg!')
  console.log('received:', sub.recv())
  setImmediate(function(){
    var cpuProfile = profiler.stopProfiling('nano')
    console.log(cpuProfile.head.children[0])
  })
},10)
