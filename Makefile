.PHONY: install update clean check test build t

LIB=$(shell pwd)/opt
libmill=$(LIB)/lib/libmill.a
sodium=$(LIB)/lib/libsodium.a
includes=-I$(LIB)/include -std=gnu99
args=--enable-shared --prefix=$(LIB)
build=./autogen.sh && ./configure $(args) && make -j 8 && make install


ALL: install

install:
	@npm i nan node-gyp
	@if [ ! -s $(libmill) ]; then cd libmill && $(build) && cd ../libsodium && $(build); fi
	@node_modules/node-gyp/bin/node-gyp.js rebuild

update:
	git submodule update --init

check:
	@make build
	@npm t

test: check

build:
	@node_modules/node-gyp/bin/node-gyp.js build

clean:
	@rm -rf lib*.gyp build node_modules opt npm-debug.log

t:
	@node_modules/node-gyp/bin/node-gyp.js build
	@node v8
