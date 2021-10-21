all: chash.o myht.c
	gcc chash.o myht.c -o myht -lm -Wall

chash.o: chash.c chash.h
	gcc -c chash.c -Wall

clean:
	rm -f *.o

purge: clean
	rm -f myht
