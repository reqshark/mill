.PHONY: clean check test

ALL:
	npm i

check:
	npm test

test:
	npm t

clean:
	rm -fr build && rm -rf node_modules
