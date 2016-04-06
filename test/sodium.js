const lib       = require('..')
module.exports  = sodium

function sodium (t) {
  t.test('nbuf', nbuf)
}

function nbuf (t) {
  t.plan(2)

  var nonce = lib.nstr()

  t.ok( nonce,   `nonce hex: ${nonce}` );
  t.is( nonce.length,  48, `nonce hex length: ${nonce.length}`   );
}
