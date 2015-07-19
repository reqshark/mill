# node.js function access to libmill
[![Build Status](https://travis-ci.org/reqshark/mill.svg?branch=master)](https://travis-ci.org/reqshark/mill)

go style concurrency for node.

an experiment

# build
```bash
$ git clone https://github.com/reqshark/mill.git && cd mill
$ make build
$ make

# linux systems need to do this extra step, making the LD PATH known at runtime
# should probably just update binding.gyp... anyway for now:
$ source lib/ldconfig.sh

# now run the build test
$ make check
```

# [test](test/readme.markdown)
tested on travis linux and osx.

## license

MIT
