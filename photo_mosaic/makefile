# Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
# para o trabalho 1 da disciplina de Programação 2 do curso de 
# Bacharelado em Ciência da Computação da UFPR

CFLAGS = -Wall -std=c99
LDLIBS = -lm

all: mosaico

mosaico: main.o image.o imgarray.o mosaic.o
	gcc main.o image.o imgarray.o mosaic.o -o mosaico $(LDLIBS) $(CFLAGS)

main.o: main.c image.h
	gcc -c main.c $(CFLAGS)

image.o: image.c image.h
	gcc -c image.c $(CFLAGS)

imgarray.o: imgarray.c imgarray.h
	gcc -c imgarray.c $(CFLAGS)

mosaic.o: mosaic.c mosaic.h
	gcc -c mosaic.c $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o

purge: clean
	rm -f mosaico 
