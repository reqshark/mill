.PHONY: clean check test

ALL:
	git submodule update --init
	npm i

check:
	npm test

test:
	npm t

clean:
	rm -fr build node_modules
