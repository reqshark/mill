var mill = require('bindings')('mill.node');
var ls = 0;

module.exports = {
  test      : mill.test,
  trace     : mill.trace
}
