all: 
	cd SP; make all
	$(MAKE) ex3
	$(MAKE) test

CC=gcc -std=c99 -Wall -Werror -pedantic-errors

ex3: main.o common.o calculate.o parse.o tree.o
	$(CC) main.o common.o calculate.o parse.o tree.o -o ex3

test: test.o common.o calculate.o parse.o tree.o
	$(CC) test.o common.o calculate.o parse.o tree.o -o test

main.o: main.c common.h tree.h parse.h calculate.h
	$(CC) -c main.c

calculate.o: calculate.c calculate.h
	$(CC) -c calculate.c

parse.o: parse.c parse.h common.h
	$(CC) -c parse.c

tree.o: tree.c tree.h common.h
	$(CC) -c tree.c

common.o: common.c common.h
	$(CC) -c common.c

common.h:
calculate.h: tree.h
parse.h: tree.h
tree.h:

clean:
	cd SP; make clean
	rm -f main.o common.o calculate.o parse.o tree.o test.o ex3 test
