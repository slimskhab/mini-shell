all: shell

shell: main.o parse.o launch.o interactive.o batch.o
	gcc main.o parse.o launch.o interactive.o batch.o -o shell

main.o: main.c
	gcc -c main.c -o main.o

parse.o: parse.c parse.h
	gcc -c parse.c -o parse.o

interactive.o: interactive.c interactive.h
	gcc -c interactive.c -o interactive.o

batch.o: batch.c batch.h
	gcc -c batch.c -o batch.o

clean:
	rm *.o shell

launch.o: launch.c launch.h
	gcc -c launch.c -o launch.o
