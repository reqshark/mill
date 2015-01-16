/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://www.wtfpl.net/>
 */

var nn            = require('bindings')('nmsg.node')
var EventEmitter  = require('events').EventEmitter
require('util').inherits( self, EventEmitter )

var sock = {
  pub             : nn.NN_PUB,
  sub             : nn.NN_SUB,
  bus             : nn.NN_BUS,
  pair            : nn.NN_PAIR,
  surv            : nn.NN_SURVEYOR,
  surveyor        : nn.NN_SURVEYOR,
  resp            : nn.NN_RESPONDENT,
  respondent      : nn.NN_RESPONDENT,
  req             : nn.NN_REQ,
  rep             : nn.NN_REP,
  pull            : nn.NN_PULL,
  push            : nn.NN_PUSH
}
var af = {
  af_sp           : nn.AF_SP,
  AF_SP           : nn.AF_SP,
  af_sp_raw       : nn.AF_SP_RAW,
  raw             : nn.AF_SP_RAW,
  af              : nn.AF_SP
}
var suggestion    = ' failed.\nsome config/opt may not be correct.\n'

module.exports    = {

  socket: function ( type, opts ) {

    if(typeof opts == 'string') opts = { fam: opts }

    //preflight check
    opts = opts || { fam: 'af' }

    //start socket
    var s = nn.Socket( af[opts.fam], sock[type])

    //throw on bad socket
    if(s < 0) throw new Error( type + ' socket' + suggestion)

    return new self(s, type, opts.fam)
  },
  wait: function (fn) {
    return function () {
      var done=false, err, res;
      fn.apply(this, Array.prototype.slice.apply(arguments).concat(cb))
      while (!done) nn.Stall(); if(err)throw err; return res
      function cb (e, r) { err=e; res=r; done=true }
    }
  }
}

function self (s, t, f) {
  this.fam        = f
  this.socket     = s
  this.type       = t
  this.open       = false
  this.close      = close
  this.bind       = bind
  this.connect    = connect
  this.send       = send
  this.recv       = recv
}

function bind (addr) {
  if (nn.Bind( this.socket, addr ) < 0)
    throw new Error(this.type+ ' bind@' + addr + suggestion)
  this.open = true

  return this
}

function connect (addr) {
  if (nn.Connect( this.socket, addr ) < 0 )
    throw new Error(this.type + ' connect@' + addr + suggestion )
  this.open = true

  return this
}

function send (msg) {
  nn.Send( this.socket,
    new Buffer(msg+'\u0000'), 0 )
}

function recv() { return nn.Recv( this.socket, 0 ) }
function close(){ return nn.Close( this.socket ) }
