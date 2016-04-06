const isBuffer  = Buffer.isBuffer
const size_t    = 32

module.exports  = ipaddr

function ipaddr (t) {
  t.test('iplocal', iplocal)
  t.test('ipremote', ipremote)
}

function iplocal (t) {
  t.plan(2)

  const iplocal = t.lib.iplocal(44445)

  t.ok( isBuffer( iplocal ),   `iplocal is node::Buffer ${iplocal}` )
  t.is( iplocal.length,  size_t, `addr length ${iplocal.length} is ${size_t}` )
}

function ipremote (t) {
  t.plan(2)

  const ipremote = t.lib.ipremote('127.0.0.1', 44445)

  t.ok( isBuffer(ipremote),  `ipremote is node::Buffer ${ipremote}` )
  t.is( ipremote.length, size_t, `addr length ${ipremote.length} is ${size_t}` )
}
