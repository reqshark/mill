var lib = require('..');

module.exports = function udp (t) {
  var ipaddr, ls, port, size_t;
  t.test('udp listen', listen);
  //t.test('udp msg', msg);
}

function listen (t) {
  t.plan(3);

  size_t = 8;
  port = 44444;
  ipaddr = lib.iplocal(port);
  ls = lib.udplisten(ipaddr);

  t.ok( Buffer.isBuffer(ls),   'listening socket (ls) is a buffer' );
  t.is( ls.length,  size_t, 'socket length is ' + size_t  );
  t.is( lib.udpport(ls),  port, 'udp listening on port ' + port  );
}

//function msg (t) {
//  t.plan(1);
//  lib.udpsend(ls, ipaddr, new Buffer('Hello, world!'));
//  t.ok(ls, 'ls: ' + ls);
//}
