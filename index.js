var mill = require('bindings')('mill.node');
var ls = 0;

module.exports = {
  test      : mill.test,
  trace     : mill.trace,
  listen    : function( addr, port ) { return new tcp('listen', addr, port) }
}

require('util').inherits(tcp, require('events').EventEmitter);

function tcp (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

tcp.prototype.listen = function(addr, port) {
  this.socket = mill.listen(ls++, addr, port);
}
