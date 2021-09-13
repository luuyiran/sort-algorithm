ALL:test

SRC:=$(wildcard *.c)

CFLAGS:=-W -Wall -std=c99 -O3 #-g -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak

test : $(SRC)
	gcc -o $@ $^ $(CFLAGS) 

.PHONY:clean
clean:
	@rm -rvf test *.txt
