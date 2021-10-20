/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

// Criar initialize_img
// Criar destroy_img

void destroy_img (image_t *img)
// Desaloca todos os espaços da imagem
{
  if (img->wid && img->hei)
  {
    free(img->m[0]);
    free(img->m);
  }
}

void next_line (FILE *fil)
// Itera para próxima linha do arquivo fil
{
  char cur;

  cur = getc(fil);
  while (cur != '\n' && cur != '\r')
    cur = getc(fil);
}

char is_number (char c)
// Se c representa um número em ASCII, retorna um caractere não nulo
// Caso contráirio, retorna 0
{
  return (c >= '0' && c <= '9');
}

char next_number (FILE *fil)
// Itera para o próximo número em ASCII não comentado ou EOF. 
// Retorna 1 caso foi encontrado um número ou 0 caso chegou a eof
{
  char cur;
  
  // Procura o próximo número não comentado ou EOF
  cur = getc(fil);
  while (!is_number(cur) && !feof(fil))
  {
    if (cur == '#')
      next_line(fil);
    cur = getc(fil);
  }

  // retorno de fracasso
  if (feof(fil))
    return 0;

  // Posiciona ponteiro logo antes do número
  fseek(fil, -1, SEEK_CUR);
  
  // retorno de sucesso 
  return 1;
}

char next_char (FILE *fil, char c)
// Itera até próxima aparição do caractere c não comentado no arquivo fil
{
  char cur;
  
  // Itera até o caractere
  cur = getc(fil);
  while(cur != c && !feof(fil))
  {
    if (cur == '#')
      next_line(fil);
    cur = getc(fil);
  }
  
  // Retorno de fracasso
  if (feof(fil))
    return 0;

  // Posiciona ponteiro logo antes do caractere
  fseek(fil, -1, SEEK_CUR);
  
  // Retorno de sucesso
  return 1;
}

void allocate_pixels(image_t *img)
// Aloca uma matriz de pixels com altura e largura especificados em img
// A matriz é um vetor contíguo que tem ponteiros para o início de cada uma
// de suas linhas
{
  // Aloca ponteiros para as linhas da matriz
  img->m = malloc(img->hei * sizeof(pixel_t *));
  if (!img)
  {
    perror("Erro ao alocar imagem!\n");
    exit(1);
  }
  
  // Aloca vetor de pixels
  img->m[0] = malloc(img->wid * img->hei * sizeof(pixel_t));
  if (!(img->m[0]))
  {
    perror("Erro ao alocar imagem!\n");
    exit(1);
  }
  
  // Faz ponteiros apontarem para inicio de cada linha
  for (int i = 1; i < img->hei; ++i)
    img->m[i] = img->m[0] + i*img->wid;

}

void read_p3(FILE *fil, image_t *img)
// Lê pixels do tipo ascii
{
  // Itera pela matriz de img armazenando valores dos pixels
  for (int i = 0; i < img->hei; ++i)
  {
    for (int j = 0; j < img->wid; ++j)
    {
      fscanf(fil, "%hhu", &(img->m[i][j].r));
      fscanf(fil, "%hhu", &(img->m[i][j].g));
      fscanf(fil, "%hhu", &(img->m[i][j].b));
    }
  }
}

void read_p6(FILE *fil, image_t *img)
// lê pixels do tipo raw
{
  int size = img->wid * img->hei * 3;

  // Como a matriz é armazenada em um vetor contínuo, todos os pixels
  // podem ser lidos com apenas um fread
  fread(img->m[0], sizeof (unsigned char), size, fil);
}

void print_image(image_t *img, FILE *output)
// Imprime header e imagem em output
{
  // Imprime header
  fprintf(output, "P%c\n%d %d\n%d\n", img->type, img->wid, img->hei, img->max);
  
  if (img->type == '3')
  {
    // Itera pela matriz rgb e imprime 3 valores dos pixels
    for (int i = 0; i < img->hei; ++i)
    {
      for (int j = 0; j < img->wid; ++j)
      {
        pixel_t *p = &(img->m[i][j]);
        fprintf(output, "%d %d %d ", p->r, p->g, p->b);
      }
      fprintf(output, "\n");
    }
  }
  else if (img->type == '6')
  {
    int size = 3 * img->wid * img->hei;
    // Imagem inteira pode ser impressa com apenas um fwrite,
    // pois pixels são armazenados de maneira contígua
    fwrite(img->m[0], sizeof(unsigned char), size, output);
  }
}

image_t * read_image(FILE *fil)
// Lê header da imagem e seus pixels e os aloca na struct de imagem
{
  image_t *img;

  // Aloca imagem
  img = malloc(sizeof(image_t));
  if (!img)
  {
    perror("Erro ao alocar a imagem!\n");
    exit(1);
  }

  // Ler tipo do arquivo
  next_char (fil, 'P');
  if (getc(fil) == 'P')
    img->type = getc(fil);
  else
  {
    perror("Erro no cabeçalho do arquivo!\n");
    exit(1);
  }
  
  // Ler largura da imagem
  if (!next_number(fil))
  {
    perror("Erro com o cabeçalho do arquivo!\n");
    exit(1);
  }
  fscanf(fil, "%d", &(img->wid));
  
  // Ler altura da imagem
  if (!next_number(fil))
  {
    perror("Erro com o cabeçalho do arquivo!\n");
    exit(1);
  }
  fscanf(fil, "%d", &(img->hei));
  
  // Alocar matriz de pixels
  allocate_pixels(img);
  
  // Ler valor de cor máximo
  if (!next_number(fil))
  {
    perror("Erro com o cabeçalho do arquivo!\n");
    exit(1);
  }

  // Ler valor maximo das componentes dos pixels
  fscanf(fil, "%d", &(img->max));
  next_line(fil);
  
  // Ler Pixels da imagem
  if (img->type == '3')
    read_p3(fil, img);
  else if (img->type == '6')
    read_p6(fil, img);
  else
  {
    fprintf(stderr, "Formato P%c não suportado!\n", img->type);
    exit(1);
  }
  
  return img;
}
