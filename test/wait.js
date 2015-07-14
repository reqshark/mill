var w = require('..').wait;


module.exports = function (t) {

  var wait = w(function (timeout, done) {
    setTimeout(done, timeout)
  })

  t.test('sleep',function(t){

    t.plan(1)

    var start = Date.now()

    wait(1000)

    var diff = Date.now() - start

    t.ok(diff, 'waited for: ' + diff + ' milliseconds')
  })
} //
