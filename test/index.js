var tape = require ('tape')
require ('fs').readdir(__dirname + '/tests', files )

function files (er, fs) {

  fs.forEach(run)

  function run (f) {

    tape ( f, require ( './tests/' + f ) )

  }

}
