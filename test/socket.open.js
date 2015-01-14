var nmsg    = require('..')
var should  = require('should')
var semver  = require('semver')
var after   = require('after')

describe('socket.open', function() {

  var fam = nmsg.AF_SP
  it('should open NN_PUB', function (done) {
    nmsg.Socket( fam, nmsg.NN_PUB).should.be.above(-1)
    done()
  })

  it('should open NN_SUB', function (done) {
    nmsg.Socket( fam, nmsg.NN_SUB).should.be.above(-1)
    done()
  })

  it('should open NN_BUS', function (done) {
    nmsg.Socket( fam, nmsg.NN_BUS).should.be.above(-1)
    done()
  })

  it('should open NN_PAIR', function (done) {
    nmsg.Socket( fam, nmsg.NN_PAIR).should.be.above(-1)
    done()
  })

  it('should open NN_SURVEYOR', function (done) {
    nmsg.Socket( fam, nmsg.NN_SURVEYOR).should.be.above(-1)
    done()
  })

  it('should open NN_RESPONDENT', function (done) {
    nmsg.Socket( fam, nmsg.NN_RESPONDENT).should.be.above(-1)
    done()
  })

  it('should open NN_REQ', function (done) {
    nmsg.Socket( fam, nmsg.NN_REQ).should.be.above(-1)
    done()
  })

  it('should open NN_REP', function (done) {
    nmsg.Socket( fam, nmsg.NN_REP).should.be.above(-1)
    done()
  })

  it('should open NN_PULL', function (done) {
    nmsg.Socket( fam, nmsg.NN_PULL).should.be.above(-1)
    done()
  })

  it('should open NN_PUSH', function (done) {
    nmsg.Socket( fam, nmsg.NN_PUSH).should.be.above(-1)
    done()
  })
})
