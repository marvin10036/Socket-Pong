algo := $(shell find ./ -name '*.c')

output: $(algo)
	gcc -g3 $(algo) -lncurses -o Socket-Pong
