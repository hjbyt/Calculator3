all: 
	cd SP; make all
	$(MAKE) SPCalculator
	$(MAKE) test

CC=gcc -std=c99 -Wall -Werror -pedantic-errors

SPCalculator: main.o common.o calculate.o parse.o tree.o SPList.o SPListElement.o hashtable.o
	$(CC) main.o common.o calculate.o parse.o tree.o SPList.o SPListElement.o hashtable.o -o SPCalculator -lm

test: test.o common.o calculate.o parse.o tree.o SPList.o SPListElement.o hashtable.o
	$(CC) test.o common.o calculate.o parse.o tree.o SPList.o SPListElement.o hashtable.o -o test -lm

main.o: main.c common.h tree.h parse.h calculate.h
	$(CC) -c main.c

calculate.o: calculate.c calculate.h
	$(CC) -c calculate.c -lm

parse.o: parse.c parse.h common.h
	$(CC) -c parse.c

tree.o: tree.c tree.h common.h
	$(CC) -c tree.c

common.o: common.c common.h
	$(CC) -c common.c
	
SPList.o: SPList.h SPList.c
	$(CC) -c SPList.c
	
SPListElement.o: SPListElement.h SPListElement.c
	$(CC) -c SPListElement.c
	
hashtable.o: hashtable.h hashtable.c
	$(CC) -c hashtable.c

common.h:
calculate.h: tree.h hashtable.h
parse.h: tree.h hashtable.h
tree.h:
SPList.h: SPListElement.h
SPListElement.h:
hashtable.h: SPListElement.h SPList.h

clean:
	cd SP; make clean
	rm -f main.o common.o calculate.o parse.o tree.o test.o SPList.o SPListElement.o hashtable.o SPCalculator test
