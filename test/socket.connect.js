var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')

describe('socket.connect', function() {

  it('should have a connect method', function(done){

    var pub = nmsg.socket('pub')

    pub.should.be.an.instanceOf(Object)
      .with.a.property('connect')
      .which.is.a.Function

    done()
  })

  it('should be called on a network address', function (done) {

    var addr = 'tcp://127.0.0.1:44445'
    var sub = nmsg.socket('sub')

    sub.should.be.an.instanceOf(Object)
      .with.a.property('open')
      .which.is.false

    sub.connect(addr)

    setTimeout(function(){

      sub.should.be.an.instanceOf(Object)
        .with.a.property('open')
        .which.is.true

      done()
    }, 100)

  })

})
