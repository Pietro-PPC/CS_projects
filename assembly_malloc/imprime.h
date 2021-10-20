#ifndef __MEUALOCADOR__
#define __MEUALOCADOR__

#define INCREMENT 4096
#define TAM_HEADER 16
void *topoInicialHeap;
void *topoBlocos;

//void iniciaAlocador();
// Armazena o topo inicial da heap

//void finalizaAlocador();
// Volta topo da heap para local inicial

long int liberaMem(void *bloco);

//void *alocaMem(long int num_bytes);

void imprimeMapa();

#endif
