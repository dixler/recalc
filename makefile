all: test

test: cursor.o stack.o tokens.o main.c
	gcc -lncurses -ltinfo -lm -g cursor.o tokens.o stack.o main.c -o test

tokens.o: tokens.c tokens.h
	gcc -c -g tokens.c

stack.o: stack.c stack.h
	gcc -c -g stack.c

cursor.o: cursor.c cursor.h
	gcc -c -g cursor.c

clean:
	rm -rf *.o
