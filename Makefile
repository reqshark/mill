.PHONY: clean check test

ALL:
	npm i

build:
	git submodule update --init
	lib/build.sh

check:
	npm test

test:
	npm t

clean:
	rm -fr build node_modules
