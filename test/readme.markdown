# build and test [![Build Status](https://circleci.com/gh/reqshark/mill/tree/master.svg?style=svg)](https://circleci.com/gh/reqshark/mill/tree/master)

`test/index.js` file runs the tests by iterating over the required contents
within this directory.

from the root directory the process is launched by node test piped to tap-spec.
```sh
$ node test | node_modules/tap-spec/bin/cmd.js
```

there are also some convenience operations via make:
```bash
$ rm -rf mill
$ git clone https://github.com/reqshark/mill.git && cd mill
$ make
$ make check
$ make clean
$ npm i && npm t
$ make build
$ make test # alias of make check
```
