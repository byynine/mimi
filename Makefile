bld/main: src/main.c | bld
	gcc\
		-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wstrict-prototypes\
		-fsanitize=leak,bounds,address,undefined\
		-fno-sanitize-recover=all\
		-g\
		-fno-omit-frame-pointer\
		-o $@ $<

bld:
	mkdir bld

run: bld/main
	@bld/main

clean:
	rm -rf bld
	rm -f ~/.local/share/mimi/data
	rm -f ~/.local/share/mimi/temp
