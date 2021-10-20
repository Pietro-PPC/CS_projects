/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include <stdio.h>
#include <stdlib.h>
#include "imgarray.h"

void allocate_imgarray (imgarray_t *tiles, DIR *storage)
// Conta quantidade de arquivos no diretório e aloca
// essa quantidade no vetor em forma de ponteiros
{
  int i = 0;
  struct dirent *cur;

  // volta para início do diretório
  rewinddir(storage);

  // itera pelo diretorio contando quantidade de arquivos
  cur = readdir(storage);
  while (cur)
  {
    i++;
    cur = readdir(storage);
  }

  i -= 2; // descontar diretorios . e ..
  tiles->size = i;

  // Aloca ponteiros para as imagens
  tiles->v = malloc (i * sizeof(image_t*));
  if (!tiles->v)
  {
    perror("Erro de alocação!\n");
    exit(1);
  }

  // volta leitura para início do diretório
  rewinddir(storage);
}

void initialize_imgarray(imgarray_t *tiles)
// Inicializa valores do vetor de imagens
{
  tiles->size = 0;
  tiles->v = NULL;
}

void destroy_imgarray(imgarray_t *tiles)
// Libera todos os espaços já alocados para o array tiles
{
  for (int i = 0; i < tiles->size; ++i)
    free(tiles->v[i]);
  free (tiles->v);
}