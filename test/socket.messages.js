var nmsg = require('..')
var should = require('should')
var semver = require('semver')

describe('socket.messages', function() {
  var addr = 'tcp://127.0.0.1:55555'
  var fam = nmsg.AF_SP
  var pub = nmsg.socket(fam, nmsg.NN_PUB)
  var sub = nmsg.socket(fam, nmsg.NN_SUB)

  nmsg.bind(pub, addr)
  nmsg.connect(sub, addr)

  it('should support messages', function (done) {

    nmsg.send(pub, 'Hello from nanomsg!')
    nmsg.recv(sub).should.equal('Hello from nanomsg!')

    done()

  })
})
