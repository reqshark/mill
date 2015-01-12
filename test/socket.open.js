var nmsg = require('..')
var should = require('should')
var semver = require('semver')

describe('socket.open', function() {

  it('should open and all AF_SP family socket types', function (done) {

    var types = [
      'NN_PUB', 'NN_SUB',
      'NN_BUS',
      'NN_PAIR',
      'NN_SURVEYOR', 'NN_RESPONDENT',
      'NN_REQ', 'NN_REP',
      'NN_PUSH', 'NN_PULL'
    ]

    var fam = nmsg.AF_SP, i=0

    while(i<=types.length){

      var socket = nmsg.socket( fam, nmsg[types[i]] )

      setTimeout(function(){

        socket.should.equal(0)

      },100)

      i++
    }

    done()

  })

})
