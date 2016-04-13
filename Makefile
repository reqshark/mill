.PHONY: clean check test build t

ALL:
	@npm i

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
