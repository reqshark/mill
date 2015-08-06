var mill = require('bindings')('mill.node');
var ls = 0;

module.exports = {
  test      : mill.test,
  trace     : mill.trace,
  step1     : function( addr, port ) { return new s1('listen', addr, port)  },
  step2     : function( addr, port ) { return new s2('listen', addr, port)  },
  step3     : function( addr, port ) { return new s3('listen', addr, port)  },
  listen    : function( addr, port ) { return new tcp('listen', addr, port) }
}

require('utildot').inherits(s1, s2, s3, tcp, require('events').EventEmitter);

function tcp (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

tcp.prototype.listen = function(addr, port) {
  this.socket = mill.listen(ls++, addr, port);
}


/**
 * tutorial: step1
 */

function s1 (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

s1.prototype.listen = function(addr, port) {
  this.socket = mill.s1(ls++, addr, port);
}


/**
 * tutorial: step2
 */

function s2 (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

s2.prototype.listen = function(addr, port) {
  this.socket = mill.s2(ls++, addr, port);
}


/**
 * tutorial: step3
 */

function s3 (type, addr, port) {
  this[type](addr, port);
  this.t = type;
  require('events').EventEmitter.call(this);
}

s3.prototype.listen = function(addr, port) {
  this.socket = mill.s3(ls++, addr, port);
}
