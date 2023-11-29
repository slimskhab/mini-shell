all: shell

shell: main.o parse.o launch.o
	gcc main.o parse.o launch.o -o shell

main.o: main.c
	gcc -c main.c -o main.o

parse.o: parse.c parse.h
	gcc -c parse.c -o parse.o

clean:
	rm *.o shell

launch.o: launch.c launch.h
	gcc -c launch.c -o launch.o
