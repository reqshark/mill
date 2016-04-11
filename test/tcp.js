module.exports = function tcp (t) {
  var ipaddr, ls, cs, as, recv, size_t, msg, sz;
  t.test('tcp listen', listen)
  t.test('tcp connect', connect)
  t.test('tcp accept', accept)
  t.test('tcp send', send)
  t.test('tcp recv', recvmsg)
  t.test('tcp sendstr', sendstr)
}

function listen (t) {
  t.plan(2)
  size_t = process.arch == 'arm' ? 4 : 8
  ipaddr = t.lib.iplocal(44444)
  ls = t.lib.tcplisten(ipaddr)

  t.ok( Buffer.isBuffer(ls),   'listening socket (ls) is a buffer' )
  t.is( ls.length,  size_t, 'socket length is ' + size_t  )
}

function connect (t) {
  t.plan(2);
  cs = t.lib.tcpconnect(ipaddr)

  t.ok( Buffer.isBuffer(cs),   'connected socket (cs) is a buffer' )
  t.is( cs.length,  size_t, 'socket length is ' + size_t  )
}

function accept (t) {
  t.plan(2);
  as = t.lib.tcpaccept(ls)

  t.ok( Buffer.isBuffer(as),   'accepted socket (as) is a buffer' );
  t.is( as.length,  size_t, 'socket length is ' + size_t  );
}

function send (t) {
  t.plan(1);
  msg = new Buffer('some low latency msgs!\n');
  sz = t.lib.tcpsend(as, msg);
  t.lib.tcpflush(as);

  t.is(sz, 23, 'tcpsend msg size is ' + sz + ' bytes');
}

function recvmsg (t) {
  t.plan(1);
  recv = t.lib.tcprecv(cs, sz);

  t.is(String(recv), String(msg), 'msg: ' + recv)
}

function sendstr (t) {
  t.plan(1)

  t.lib.box_keypair()

  sz = t.lib.tcpsendstr(as, 'hello encrypted socket!')
  t.lib.tcpflush(as)
  recv = t.lib.tcprecv(cs, sz)

  t.is(String(recv).length, sz, 'msg: ' + recv)

  t.lib.tcpclose(as)
}
