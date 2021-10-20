#include <stdio.h>
#include <unistd.h>
#include "meuAlocador.h"

void iniciaAlocador()
// Armazena o topo inicial da heap
{
  printf("\n");
  topoInicialHeap = sbrk(0);
  topoBlocos = topoInicialHeap+24;
}

void finalizaAlocador()
// Volta topo da heap para local inicial
{
  brk(topoInicialHeap);  
}

long int liberaMem(void *bloco)
{
  bloco -= HEADER_SIZE;
  long int *aux = bloco;
  *aux = 0;
  return 0;
}

void *alocaMem(long int num_bytes)
{
  // dispensa entradas inválidas
  
  if (num_bytes <= 0)
    return NULL;
  
  sbrk(24);
  
  // Procura bloco vazio
  long int bestFit = 0;
  long int *cabecalho;
  void *iterator = topoInicialHeap;
  void *comecoBloco = topoBlocos;
  long int *isDisp;
  long int disp;
  long int mult;
  long int excesso;
  long int *bloco;
  
  
  while( iterator < topoBlocos )
  {
    cabecalho = iterator;
    if (!cabecalho[0] &&
      (cabecalho[1] >= num_bytes) && 
      ((cabecalho[1] < bestFit) || !bestFit) ){

      bestFit = cabecalho[1];
      comecoBloco = iterator;
    }
    iterator += HEADER_SIZE + cabecalho[1];
  }

  // Retorna caso foi achado algum bloco
  if (bestFit){
    isDisp = comecoBloco;
    *isDisp = 1;
    return comecoBloco + HEADER_SIZE;
  }

  // Aloca quanta memória a mais for necessária
  
  disp = sbrk(0) - topoBlocos;
  if (HEADER_SIZE + num_bytes > disp){
    excesso = HEADER_SIZE + num_bytes - disp; 
    mult = 1 + ((excesso-1)/INCREMENT);
    sbrk(INCREMENT * mult);
  }
  
  // Insere cabeçalho do bloco
  bloco = topoBlocos;
  bloco[0] = 1;
  bloco[1] = num_bytes;

  // Atualiza variável topoBlocos
  topoBlocos += HEADER_SIZE + num_bytes;

  return (void *) bloco + HEADER_SIZE;
  /**/
}

void imprimeHeap(){
  void *iterator = topoInicialHeap;
  printf("\nESTADO DA HEAP:\n");
  while ( iterator < topoBlocos ){
    long int *a = iterator;
    printf("%ld %ld\n", a[0], a[1]);
    iterator += HEADER_SIZE + a[1];
  }
}