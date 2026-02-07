bld/main: src/main.c | bld
	gcc -o $@ $<

bld:
	mkdir -p bld

run: bld/main
	@bld/main

clean:
	rm -rf bld/
