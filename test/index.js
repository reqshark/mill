var tape = require ('tape')

tape('==================== mill testsuite summary ====================', tests)
function tests (t) {
  t.test('===== ipaddr buffers =====', require('./ipaddr'))
  t.test('===== tcp library =====', require('./tcp'))
  t.test('===== udp library =====', require('./udp'))
  t.test('===== sodium library =====', require('./sodium'))
}

tape.Test.prototype.lib = require('..')
