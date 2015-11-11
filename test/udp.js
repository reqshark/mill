var lib = require('..');

module.exports = function udp (t) {
  t.test('udp msgs', listen);
}

function listen (t) {
  t.plan(5);

  var buf = new Buffer('Hello, world!');
  var size_t = 8;
  var port = 44444;

  var ipaddr = lib.iplocal(port);
  var ls = lib.udplisten(ipaddr);

  t.ok( Buffer.isBuffer(ls),   'listening socket (ls) is a buffer' );
  t.is( ls.length,  size_t, 'socket length is ' + size_t  );
  t.is( lib.udpport(ls),  port, 'udp listening on port ' + port  );

  var i = 10000;
  var msgs = [];

  while(i--){
    lib.udpsend(ls, ipaddr, buf);
    msgs.push(lib.udprecv(ls, 13).buf);
  }

  var bufferLoss = [];
  var validator = String(buf);
  var l = msgs.length;

  t.ok(l, 'total udp msgs sent and recv\'d: ' + ( l/1000 )+'K');

  while(l--) if (String(msgs[l]) !== validator)
    bufferLoss.push(msgs[l]);

  t.ok(validator, 'total message loss: '+ bufferLoss.length);
}
