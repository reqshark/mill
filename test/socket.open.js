var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')

describe('socket.open', function() {

  it('should open NN_PUB', function (done) {

    nmsg.socket('pub').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_SUB', function (done) {

    nmsg.socket('sub').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_BUS', function (done) {

    nmsg.socket('bus').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_PAIR', function (done) {

    nmsg.socket('pair').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_SURVEYOR', function (done) {

    nmsg.socket('surv').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_RESPONDENT', function (done) {

    nmsg.socket('resp').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_REQ', function (done) {

    nmsg.socket('req').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_REP', function (done) {

    nmsg.socket('rep').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_PULL', function (done) {

    nmsg.socket('pull').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

  it('should open NN_PUSH', function (done) {

    nmsg.socket('push').should.be.an.instanceOf(Object)
      .and.have.property('socket')
      .which.is.above(-1)

    done()
  })

})
