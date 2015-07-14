/*
 * similar to yield, wait pauses the node/iojs default runloop. it will block
 * everything else in its surrounding context until its completion flag is set:
 * calling done() resumes contextual program flow back to normal asyc javascript
 */
var binding = require('bindings')('mill.node')

module.exports = {
  wait  :   wait,
  test  :   binding.test
}

function wait(fn) {
  return function () {
    var done=false, err, res;
    fn.apply(this, Array.prototype.slice.apply(arguments).concat(cb))
    while (!done) binding.stall(); if(err)throw err; return res
    function cb (e, r) { err=e; res=r; done=true }
  }
}
