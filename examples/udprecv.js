// udp recv
var m = require('..');
var s = m.udplisten(5555);

process.stdout.write('udp socket listening on port: '
  + m.udpport(s) + '\n');

while(1)
  process.stdout.write(m.udprecv(s, 13) + '\n');
