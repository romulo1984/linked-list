CC=gcc
CFLAGS=-I.
DEPS = lista.h
OBJ = prog.o lista.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prog: prog.c lista.c
	$(CC) -o prog prog.c lista.c -I.

run: prog
	./prog ${input}
