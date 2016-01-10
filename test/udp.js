var lib = require('..');

module.exports = function udp (t) {
  t.test('udp msgs', listen);
}

function listen (t) {
  t.plan(6);

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

  /* send and recv 10K udp packets */
  while(i--){
    lib.udpsend(ls, ipaddr, buf);
    var udprecv = lib.udprecv(ls, 13, 10);
    msgs.push(udprecv.buf);
  }

  /* test ipadrstr on the last inbound udp packet */
  t.is(udprecv.addr, '127.0.0.1', 'confirmed localhost addrstr: 127.0.0.1');

  var bufferLoss = [];
  var validator = String(buf);
  var l = msgs.length;

  t.ok(l, 'total udp msgs sent and recv\'d: ' + ( l/1000 )+'K');

  /* observe any UDP packet loss. expect zero loss over local interface */
  while(l--) if (String(msgs[l]) !== validator)
    bufferLoss.push(msgs[l]);

  t.ok(validator, 'total message loss: '+ bufferLoss.length);
}
