var lib         = require('..');
var isBuffer    = Buffer.isBuffer;
var size_t      = 8;

module.exports  = ipaddr;

function ipaddr (t) {
  t.test('iplocal', iplocal);
  t.test('ipremote', ipremote);
}

function iplocal (t) {
  t.plan(2);

  var iplocal = lib.iplocal(44445);

  t.ok( isBuffer(iplocal),   'iplocal is a node::Buffer' );
  t.is( iplocal.length,  size_t, 'iplocal addr length is ' + size_t  );
}

function ipremote (t) {
  t.plan(2);

  var ipremote = lib.ipremote('127.0.0.1', 44445);

  t.ok( isBuffer(ipremote),  'ipremote is also a Buffer' );
  t.is( ipremote.length, size_t, 'ipremote addr length is 8' );
}
