module.exports  = sodium

function sodium (t) {
  t.test( 'sodium_version', sodium_version)
  t.test( 'print crypto box primitive', box_primitive )
  t.test( 'nbuf', nbuf )
  t.test( 'socket buffer integrity check', socketbuffers )
}

function sodium_version (t) {
  t.plan(1)
  t.ok(t.lib.sodium_version(), `sodium version: ${t.lib.sodium_version()}`)
}

function box_primitive (t) {
  t.plan(1)
  t.ok(t.lib.box_primitive(), `crypto box primitive: ${t.lib.box_primitive()}`)
}

function nbuf (t) {
  t.plan(2)

  var nonce     = t.lib.nstr()
  t.ok( nonce,   `nonce hex: ${nonce}` )
  t.is( nonce.length,  48, `nonce hex length: ${nonce.length}`   )
}

function socketbuffers (t) {
  t.plan(20)

  var port      = 5678
  var i         = 20
  var bufs      = {}
  var sockets   = []

  while (i--) {
    var ipaddr = t.lib.iplocal(port++)
    var s = t.lib.udplisten(ipaddr)
    bufs[ s ] = i
    sockets.push(s)
  }

  sockets
    .reverse()
    .map( s => t.is( ++i,  bufs[s], `socketbuffer # ${i} == ${bufs[s]}` ) )
}
