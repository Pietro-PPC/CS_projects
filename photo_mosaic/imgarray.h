/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include <dirent.h>
#include "image.h"

#ifndef __IMGARRAY__
#define __IMGARRAY__

typedef struct imgarray_s
{
  int size;    // informa quantidade de imagens armazenadas
  image_t **v; // vetor propriamente dito
} imgarray_t;

void allocate_imgarray (imgarray_t *tiles, DIR *storage);

void initialize_imgarray(imgarray_t *tiles);

void destroy_imgarray(imgarray_t *tiles);

#endif