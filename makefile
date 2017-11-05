all: test

test: stack.o main.c tokens.o
	gcc -g tokens.o stack.o main.c -o test

tokens.o: tokens.c tokens.h
	gcc -c -g tokens.c

stack.o: stack.c stack.h
	gcc -c -g stack.c

clean:
	rm -rf *.o
