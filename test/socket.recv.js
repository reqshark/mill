var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')

var addr = 'tcp://127.0.0.1:54444'
var pub = nmsg.socket('pub')
var sub = nmsg.socket('sub')

describe('socket.recv', function() {

  pub.bind(addr)
  sub.connect(addr)

  it('should have a send method', function(done){

    pub.should.be.an.instanceOf(Object)
      .with.a.property('send')
      .which.is.a.Function

    done()
  })

  it('should have a recv method', function(done){

    sub.should.be.an.instanceOf(Object)
      .with.a.property('recv')
      .which.is.a.Function

    done()
  })

  it('should send and receive msgs', function (done) {

    pub.send('hello from nanømsg!')

    setTimeout(function(){

      sub.recv().should.equal('hello from nanømsg!')
      done()
    }, 10)

  })

})
