var lib  = require('./build/Release/mill.node');
module.exports = ctx = {
  test          : lib.test,
  trace         : lib.trace,
  tcplisten     : lib.tcplisten,
  tcpaccept     : lib.tcpaccept,
  tcpconnect    : lib.tcpconnect,
  tcpsend       : lib.tcpsend,
  tcpflush      : lib.tcpflush,
  tcprecv       : lib.tcprecv,
  tcprecvuntil  : lib.tcprecvuntil,
  tcpport       : lib.tcpport,
  tcpclose      : lib.tcpclose,
}
