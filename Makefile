.PHONY: clean check test

ALL: loop

configure:
	git submodule update --init
	@cd libsodium; ./autogen.sh && ./configure
	@node defines.js

loop:
	@npm i nan node-gyp
    ifeq (,$(wildcard libsodium.gyp))
	@make configure
    endif
	@node_modules/node-gyp/bin/node-gyp.js rebuild

check:
	npm test

test:
	npm t

clean:
	rm -fr build node_modules libsodium.gyp
