/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include "image.h"
#include "imgarray.h"

#ifndef __MOSAIC__
#define __MOSAIC__

// definir macro square()

void make_mosaic(image_t *main_img, imgarray_t *tiles);

void avg_color(image_t *img, int start_lin, int start_col, int tile_size, pixel_t *p);

void assign_pixel (pixel_t *p1, pixel_t *p2);

#endif