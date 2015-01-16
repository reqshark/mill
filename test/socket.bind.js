var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')

describe('socket.bind', function() {

  it('should have a bind method', function(done){

    var pub = nmsg.socket('pub')

    pub.should.be.an.instanceOf(Object)
      .with.a.property('bind')
      .which.is.a.Function

    done()
  })

  it('should be called on a network address', function (done) {

    var addr = 'tcp://127.0.0.1:44444'
    var sub = nmsg.socket('sub')

    sub.should.be.an.instanceOf(Object)
      .with.a.property('open')
      .which.is.false

    sub.bind(addr)

    setTimeout(function(){

      sub.should.be.an.instanceOf(Object)
        .with.a.property('open')
        .which.is.true

      done()
    }, 30)

  })

})
