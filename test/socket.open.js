var nmsg = require('..')
var should = require('should')
var semver = require('semver')

describe('socket.open', function() {

  it('should open all socket types', function (done) {

    var types = [
      'NN_PUB', 'NN_SUB',
      'NN_BUS',
      'NN_PAIR',
      'NN_SURVEYOR', 'NN_RESPONDENT',
      'NN_REQ', 'NN_REP',
      'NN_PUSH', 'NN_PULL'
    ]

    var fam = nmsg.AF_SP, i=0

    while(i<=types){
      var socket = nmsg.socket( fam, nmsg[types[i]] )
      socket.should.equal(0)
    }

    done()

  })
})
