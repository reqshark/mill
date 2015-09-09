var mill = require('bindings')('mill.node');
var ls = 0;
var cs = 0;

module.exports = {
  test      : mill.test,
  trace     : mill.trace,
//  listen    : function( addr, port ) { return new tcp('listen',   addr, port) }
  connect   : function( addr, port ) { return new tcp('connect',  addr, port) }
}

require('utildot').inherits(tcp, require('events').EventEmitter);

function tcp (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

tcp.prototype.connect = function(addr, port) {
  this.socket = mill.connect(addr, port, cs++);
  return this;
}

//tcp.prototype.listen = function(addr, port) {
//  this.socket = mill.listen(addr, port, ls++);
//}
