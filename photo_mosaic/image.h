/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#ifndef __IMAGE__
#define __IMAGE__

typedef struct pixel_s
{
  unsigned char r; // valor red
  unsigned char g; // valor green
  unsigned char b; // valor blue
} pixel_t;

typedef struct image_s
{
  char type;          // '3' para P3, '6' para P6
  int wid;            // largura da imagem
  int hei;            // altura da imagem
  int max;            // max
  pixel_t mean_color; // calcul
  pixel_t **m;        // matriz de pixels
} image_t;

// Escreve a imagem img em seu formato especificado (P3 ou P6) no arquivo output
void print_image (image_t *img, FILE *output);

// Lê tipo, altura, largura e pixels da imagem e retorna a struct
image_t * read_image(FILE *fil);

// Inicializa valores da imagem
void initialize_img(image_t *img);

// Desaloca espaços alocados da imagem
void destroy_img (image_t *img);

#endif
