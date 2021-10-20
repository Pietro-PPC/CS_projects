/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mosaic.h"
#include "image.h"

#define SQUARE(x) (x)*(x)

void avg_color(image_t *img, int start_lin, int start_col, int tile_size, pixel_t *p)
// Calcula cor média da porção quadrada da imagem que começa em start_lin e start_col e
// que tem lado tile_size (ou menor, caso a imagem acabe antes).
// A cor média é a raiz quadrada da média de quadrados de cada componente do pixel.
{
  int mean_r = 0, mean_g = 0, mean_b = 0;
  double num_pixels = 0.0;
  
  // Acumula somas de quadrados
  for (int i = start_lin; i < start_lin + tile_size && i < img->hei; ++i)
  {
    for (int j = start_col; j < start_col + tile_size && j < img->wid; ++j)
    {
      mean_r += SQUARE(img->m[i][j].r);
      mean_g += SQUARE(img->m[i][j].g);
      mean_b += SQUARE(img->m[i][j].b);
      num_pixels += 1.0;
    }
  }

  // Calcula media de qudrados
  mean_r /= num_pixels;
  mean_g /= num_pixels;
  mean_b /= num_pixels;

  // Armazena raizes das medias no pixel p
  p->r = round(sqrt((double) mean_r));
  p->g = round(sqrt((double) mean_g));
  p->b = round(sqrt((double) mean_b));
}

void assign_pixel (pixel_t *p1, pixel_t *p2)
// Pixel p1 assume valores do pixel p2
{
  p1->r = p2->r;
  p1->g = p2->g;
  p1->b = p2->b;
}

// Calcula distancia entre os pixels por redmean 

double redmean(pixel_t *p1, pixel_t *p2)
{
  return sqrt( SQUARE(p1->r - p2->r) + SQUARE(p1->g - p2->g) + SQUARE(p1->b - p2->b));
}

/*
double redmean(pixel_t *p1, pixel_t *p2)
{
  int r1 = (double) p1->r;
  int g1 = (double) p1->g;
  int b1 = (double) p1->b;
  int r2 = (double) p2->r;
  int g2 = (double) p2->g;
  int b2 = (double) p2->b;

  // Retora redmean entre os pixels
  return sqrt( (2.0 + (r1+r2)/(2.0*256.0) ) * pow((r2-r1), 2.0) +
               4.0 * pow((g2-g1), 2) + 
               (2.0 + ( 255.0 - ((r1+r2)/2) )/256.0) * pow((b2-b1), 2) );
}*/

int best_tile (imgarray_t *tiles, pixel_t *avg)
// Itera pelo array de pastilhas e retorna o índice da pastilha de cor média
// mais próxima de avg
{
  int ret = 0;
  double min_value, curr;

  // Pega distancia para primeira imagem
  min_value = redmean(&(tiles->v[0]->mean_color), avg);

  // itera pelo vetor e acha indice da pastilha mais proxima
  for (int i = 1; i < tiles->size; ++i)
  {
    curr = redmean(&(tiles->v[i]->mean_color), avg);
    if (curr < min_value)
    {
      min_value = curr;
      ret = i;
    }
  }

  return ret;
}

void place_tile(image_t *tile, image_t *main_img, int start_lin, int start_col)
// Posiciona pastilha tile na imagem main_img, começando em start_lin e start_col,
// cortando o excesso
{
  int tile_size = tile->wid;
  for (int i = start_lin; i < start_lin + tile_size && i < main_img->hei; ++i)
  {
    for (int j = start_col; j < start_col + tile_size && j < main_img->wid; ++j)
    {
      assign_pixel(&(main_img->m[i][j]), &(tile->m[i - start_lin][j - start_col]));
    }
  }
}

void make_mosaic(image_t *main_img, imgarray_t *tiles)
// Constroi mosaico com pastilhas armazenadas em tiles em cima da imagem main_img
{
  int tile_size = tiles->v[0]->wid;
  int best;
  pixel_t avg;

  // itera por porções quadradas de lado tile_size (ou menores) da imagem principal
  for (int i = 0; i < main_img->hei; i += tile_size)
  {
    for (int j = 0; j < main_img->wid; j += tile_size)
    {
      // calcula cor média da porção da imagem
      avg_color (main_img, i, j, tile_size, &avg);
      // procura pastilha mais proxima da media da imagem
      best = best_tile (tiles, &avg);
      // substitui porção da imagem por pastilha que se encaixa melhor
      place_tile (tiles->v[best], main_img, i, j);
    }
  }
}