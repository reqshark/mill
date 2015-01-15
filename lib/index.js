var nn          = require('bindings')('nmsg.node')
var evts        = require('events')
module.exports  = nmsg = new evts.EventEmitter()

var sock = {
  pub         : nn.NN_PUB,
  sub         : nn.NN_SUB,
  bus         : nn.NN_BUS,
  pair        : nn.NN_PAIR,
  sur         : nn.NN_SURVEYOR,
  surveyor    : nn.NN_SURVEYOR,
  res         : nn.NN_RESPONDENT,
  respondent  : nn.NN_RESPONDENT,
  req         : nn.NN_REQ,
  rep         : nn.NN_REP,
  pull        : nn.NN_PULL,
  push        : nn.NN_PUSH
}

var af = {
  af_sp       : nn.AF_SP,
  AF_SP       : nn.AF_SP,
  af_sp_raw   : nn.AF_SP_RAW,
  raw         : nn.AF_SP_RAW,
  af          : nn.AF_SP
}

nmsg.socket = function(type,opts){
  var suggestion  = ' socket failed: options may not be correct.'

  if(typeof opts == 'string')
    opts = {fam:opts}

  //preflight check
  opts = opts || {fam:'af'}

  //start socket
  var s = nn.Socket( af[opts.fam], sock[type])

  if(s < 0)
    throw new Error(type+suggestion)

  return s
}
