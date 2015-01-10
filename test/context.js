var should = require('should')

describe('context', function() {

  it('should appreciate zero context', function (done) {
    var copy = 'ø'
    copy.should.equal('ø')

    done()

  })

})
