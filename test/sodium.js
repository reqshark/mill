module.exports  = sodium

function sodium (t) {
  t.test( 'sodium_version', sodium_version )
  t.test( 'print crypto box primitive', box_primitive )
  t.test( 'get and set keys', getandsetkeys )
  t.test( 'set a crypto box keypair', box_keypair )
  t.test( 'nbuf', nbuf )
}

function sodium_version (t) {
  t.plan(1)
  t.ok(t.lib.sodium_version(), `sodium version: ${t.lib.sodium_version()}`)
}

function box_primitive (t) {
  t.plan(1)
  t.ok(t.lib.box_primitive(), `crypto box primitive: ${t.lib.box_primitive()}`)
}

function getandsetkeys (t) {
  t.plan(2)

  const pk = '6230325a03ca7507490463f1df09286d8bff3d81deafcf2e3c26003d8bc50c0e'
  const sk = '3f9899a6722641c40e70b8bafed4486ca301b5f8325b01a2e0af80f830daa249'

  /* set the public key (pk) and secret key (sk) */
  t.lib.setk( pk, sk )

  /* now get them back */
  const key = t.lib.getk()

  t.is( key.pk, pk, `public key set: ${key.pk} get: ${pk}`)
  t.is( key.sk, sk, `secret key set: ${key.sk} get: ${sk}`)
}

function box_keypair (t) {
  t.plan(2)

  /* set a box pair of public (pk) and secret (sk) keys */
  t.lib.box_keypair()

  /* now get them back */
  t.ok( t.lib.getk().pk, `box pair public key: ${t.lib.getk().pk}`)
  t.ok( t.lib.getk().sk, `box pair secret key: ${t.lib.getk().sk}`)
}

function nbuf (t) {
  t.plan(2)

  var nonce     = t.lib.nstr()
  t.ok( nonce,   `nonce hex: ${nonce}` )
  t.is( nonce.length,  48, `nonce hex length: ${nonce.length}`   )
}
