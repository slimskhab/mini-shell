all: main.o parse.o
	gcc -o shell main.o

main.o: main.c
	gcc -o main.o -c main.c

parse.o: parse.c parse.h
	gcc -o parse.o -c parse.c

clean:
	rm -f shell main.o parse.o

