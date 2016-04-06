# build and test [![Build Status](https://circleci.com/gh/reqshark/mill/tree/master.svg?style=svg)](https://circleci.com/gh/reqshark/mill/tree/master)

`test/index.js` file runs the tests by iterating over the required contents
within this directory.

from the root directory the process is launched by node test piped to tap-spec.
```js
$ node test | tap-spec
```

there are also some convenience operations via make:
```bash
$ git clone https://github.com/reqshark/mill.git && cd mill
$ make clean && make && make check
```

these commands are respectively:
```bash
$ make clean
$ npm i
$ npm t
```
