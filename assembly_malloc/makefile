all: main.o meuAlocador.o
	gcc -static main.o meuAlocador.o -o main

debug: main.o meuAlocador.o 
	gcc -static main.o meuAlocador.o -o main -g

main.o: main.c
	gcc -c main.c -o main.o -g

meuAlocador.o: meuAlocador.h meuAlocador.s
	as meuAlocador.s -o meuAlocador.o -g

clean:
	rm *.o

purge: 
	rm *.o
	rm main
