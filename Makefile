.PHONY: clean check test

ALL: loop

configure:
	@git submodule update --init
	@cd libsodium; ./autogen.sh && ./configure --disable-shared
	@cd libmill; ./autogen.sh && ./configure --disable-shared
	@node configure

loop:
	@npm i nan node-gyp
    ifeq (,$(wildcard libsodium.gyp))
	@make configure
    endif
	@node_modules/node-gyp/bin/node-gyp.js rebuild

check:
	@npm t

test: check

clean:
	@rm -rf lib*.gyp build node_modules
