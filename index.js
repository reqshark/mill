var mill  = require('bindings')('mill.node');
var ls    = 1;
var cs    = 1;

module.exports = ctx = {
  err       : new (require('events').EventEmitter)(),
  test      : mill.test,
  trace     : mill.trace,
//  listen    : function( addr, port ) { return new tcp('listen',   addr, port) }
  connect   : function( addr, port ) {
    return new tcp('connect',  addr, port)
  }
}

require('utildot').inherits(tcp, require('events').EventEmitter);

function tcp (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

tcp.prototype.connect = function(addr, port) {
  //store a copy of the connection value
  this.s = Number(String(cs++));

  //try to connect
  this.socket = mill.connect(addr, port, this.s);

  if (this.socket)
    return this;

  // if this.socket is zero or falsey, we had a connection error.
  this.err = true;
  ctx.err.emit('err', {socket: this, type: 'connect'});
  return this;
}

//tcp.prototype.send = function(addr, port) {
//  this.socket = mill.connect(addr, port, cs++);
//  return this;
//}

//tcp.prototype.listen = function(addr, port) {
//  this.socket = mill.listen(addr, port, ls++);
//}
