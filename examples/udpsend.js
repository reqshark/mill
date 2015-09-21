// udp send
var s = require('..').udplisten(5555);
require('..').udpsend(s, '10.0.1.13', 5555, new Buffer('Hello, world!'));
