require ('tape')('============== mill testsuite summary ==============', tests)

function tests (t) {
  t.test('===== callbacks ==========', require('./callbacks'))
  t.test('===== ipaddr buffers =====', require('./ipaddr'))
  t.test('===== socket buffers =====', require('./bufs'))
  t.test('===== tcp library ========', require('./tcp'))
  t.test('===== udp library ========', require('./udp'))
  t.test('===== sodium library =====', require('./sodium'))
}

require ('tape').Test.prototype.lib = require('..')
