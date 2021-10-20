#include <stdio.h>
#include <unistd.h>
#include "meuAlocador.h"

void imprime(){
  void *iterator = topoInicialHeap;
  printf("ESTADO DA HEAP:\n");
  while ( iterator < topoBlocos ){
    long int *a = iterator;
    printf("%ld %ld\n", a[0], a[1]);
    iterator += HEADER_SIZE + a[1];
  }
}

int main()
{
  long int qt_bytes;
  long int *ptr1, *ptr2;

  iniciaAlocador();
  ptr2 = alocaMem(15);
  imprimeMapa();
  liberaMem(ptr2);
  imprimeMapa();

  ptr1 = alocaMem(20);
  imprimeMapa();
  liberaMem(ptr1);
  imprimeMapa();

  alocaMem(10);
  imprimeMapa();
  imprimeMapa();
  finalizaAlocador();

  return 0;
}
