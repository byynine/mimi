bld/main: src/main.c src/inc/*.c | bld
	gcc -Wall -Wextra -o $@ $<

bld:
	mkdir -p bld

run: bld/main
	@bld/main

clean:
	rm -rf bld/
	rm -f mimi.json
