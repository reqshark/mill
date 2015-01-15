var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')
var after   = require('after')

describe('socket.open', function() {

  it('should open NN_PUB', function (done) {
    nmsg.socket('pub').should.be.above(-1)
    done()
  })

  it('should open NN_SUB', function (done) {
    nmsg.socket('sub').should.be.above(-1)
    done()
  })

  it('should open NN_BUS', function (done) {
    nmsg.socket('bus').should.be.above(-1)
    done()
  })

  it('should open NN_PAIR', function (done) {
    nmsg.socket('pair').should.be.above(-1)
    done()
  })

  it('should open NN_SURVEYOR', function (done) {
    nmsg.socket('surveyor').should.be.above(-1)
    done()
  })

  it('should open NN_RESPONDENT', function (done) {
    nmsg.socket('respondent').should.be.above(-1)
    done()
  })

  it('should open NN_REQ', function (done) {
    nmsg.socket('req').should.be.above(-1)
    done()
  })

  it('should open NN_REP', function (done) {
    nmsg.socket('rep').should.be.above(-1)
    done()
  })

  it('should open NN_PULL', function (done) {
    nmsg.socket('pull').should.be.above(-1)
    done()
  })

  it('should open NN_PUSH', function (done) {
    nmsg.socket('push').should.be.above(-1)
    done()
  })
})
