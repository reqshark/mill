require ('tape')('============== mill testsuite summary ==============', tests)

function tests (t) {
  t.test('===== ipaddr buffers =====', require('./ipaddr'))
  t.test('===== tcp library ========', require('./tcp'))
  t.test('===== udp library ========', require('./udp'))
  t.test('===== callbacks ==========', require('./callbacks'))
  t.test('===== sodium library =====', require('./sodium'))
}

require ('tape').Test.prototype.lib = require('..')
