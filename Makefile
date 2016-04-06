.PHONY: clean check test build

ALL: loop

configure:
	@git submodule update --init
	@cd libsodium; ./autogen.sh && ./configure --disable-shared
	@cd libmill; ./autogen.sh && ./configure --disable-shared
	@node configure

loop:
	@npm i nan node-gyp
    ifeq (,$(wildcard libmill.gyp))
	@make configure
    endif
	@node_modules/node-gyp/bin/node-gyp.js rebuild

check:
	@make build
	@npm t

test: check

build:
	@node_modules/node-gyp/bin/node-gyp.js build

clean:
	@rm -rf lib*.gyp build node_modules
