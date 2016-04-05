.PHONY: clean check test

ALL: sodium

configure:
	git submodule update --init
	@cd libsodium; ./autogen.sh
	@cd libsodium; ./configure
	@node defines.js

sodium:
	@npm i nan node-gyp
    ifeq (,$(wildcard libsodium.gyp))
	@echo Running make configure
	@make configure
    endif
	@node_modules/node-gyp/bin/node-gyp.js rebuild

check:
	npm test

test:
	npm t

clean:
	rm -fr build node_modules libsodium.gyp
