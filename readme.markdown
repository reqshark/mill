# node.js function access to libmill
[![Build Status](https://travis-ci.org/reqshark/mill.svg?branch=master)](https://travis-ci.org/reqshark/mill)

go style concurrency for node.

an experiment

```bash
$ npm i libmill

# or
$ git clone https://github.com/reqshark/mill.git && cd mill
$ git submodule update --init
$ npm i && npm t
```

# build

```bash
$ make
```

# test
```bash
$ make clean && make && make check

# or
$ rm -rf node_modules build
$ npm i
$ npm t
```

## license

MIT

<sub>*tested on travis linux and osx.*</sub>
