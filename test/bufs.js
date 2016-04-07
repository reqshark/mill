module.exports  = callbacks

function callbacks (t) {
  t.test( 'socket buffer integrity check', socketbuffers )
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
