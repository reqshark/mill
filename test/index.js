var tape = require ('tape')
require ('fs').readdir(__dirname, files )

function files (er, fs) {

  fs.shift()

  fs.forEach(specRunner)

  function specRunner (f) {

    tape ( f, require ( './' + f ) )

  }

}
