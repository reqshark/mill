TESTS = $(wildcard test/test.*.js)
MOCHA = node_modules/.bin/mocha

ALL:
	npm i

check:
	$(MOCHA) --expose-gc --slow 2000 --timeout 600000

clean:
	rm -fr build
	rm -rf node_modules

.PHONY: clean check
