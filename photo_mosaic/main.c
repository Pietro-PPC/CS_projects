/*
  Código desenvolvido por Pietro Polinari Cavassin, GRR20190430
  para o trabalho 1 da disciplina de Programação 2 do curso de 
  Bacharelado em Ciência da Computação da UFPR
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include "image.h"
#include "imgarray.h"
#include "mosaic.h"

#define MAX_FILENAME 101

void read_tiles (imgarray_t *tiles, char *tile_dir)
// Armazena as pastilhas do diretorio tile_dir no array tiles
{
  DIR *storage;
  FILE *cur_f;
  struct dirent *cur_sd;
  char filename[MAX_FILENAME];
  int i;
  pixel_t aux;

  // Abre diretorio de pastilhas tile_dir
  storage = opendir(tile_dir);
  if (!storage)
  {
    perror("Diretorio nao aberto com sucesso!\n");
    exit(1);
  }

  // Aloca vetor de pastilhas
  allocate_imgarray(tiles, storage);

  // Itera pelo diretório, abre arquivos de cada pastilha e armazena no vetor de pastilhas
  i = 0;
  cur_sd = readdir(storage);
  while(cur_sd)
  {
    if (cur_sd->d_name[0] != '.')
    {
      // Constroi nome do arquivo de imagem atual a ser aberto
      strcpy(filename, tile_dir);
      strcat(filename, "/");
      strcat(filename, cur_sd->d_name);
      
      // Abre arquivo atual de imagem
      cur_f = fopen(filename, "r");
      if (!cur_f)
      {
        perror("erro ao tentar abrir imagem!\n");
        exit(1);
      }
      
      // Lê imagem e calcula sua cor média
      tiles->v[i] = read_image(cur_f);
      avg_color(tiles->v[i], 0, 0, tiles->v[i]->wid, &aux);
      assign_pixel(&(tiles->v[i]->mean_color), &aux);
      fclose(cur_f);
      ++i;
    }
    // Lê próximo arquivo do diretório
    cur_sd = readdir(storage);
  }

  closedir(storage);
}

int main(int argc, char **argv)
{
  FILE *main_img_i = stdin;
  FILE *main_img_o = stdout;
  image_t *main_img = NULL;
  imgarray_t tiles;
  char img_input[MAX_FILENAME];
  char img_output[MAX_FILENAME];
  char tile_dir[MAX_FILENAME];

  // Armazena valores padrões da entrada, saida e do diretório nessas strings
  strcpy(img_input, "stdin");
  strcpy(img_output, "stdout");
  strcpy(tile_dir, "./tiles");

  // Analisa opções de execução do arquivo
  for (int i = 1; i < argc; i += 2)
  {
    // Testa se os argumentos estão em pares 
    if (argc < i+2 || strlen(argv[i]) < 2)
    {
      perror ("Erro nas opções de chamada do programa!\n");
      exit(1);
    }

    // testa argumento e assinala arquivo ou diretorio associado a ele à respectiva string
    if (!strcmp(argv[i], "-i"))
      strcpy(img_input, argv[i+1]);
    else if (!strcmp(argv[i], "-o"))
      strcpy(img_output, argv[i+1]);
    else if (!strcmp(argv[i], "-p"))
      strcpy(tile_dir, argv[i+1]);
    else
    {
      perror ("Erro nas opções de chamada do programa!\n");
      exit(1);
    }
  }

  // Abrir arquivo de imagem
  if (strcmp(img_input, "stdin"))
  {
    main_img_i = fopen(img_input, "r");
    if (!main_img_i)
    {
      perror("Erro ao abrir o arquivo de imagem!\n");
      exit(1);
    }
  }

  // Abrir arquivo de saida da imagem
  if (strcmp(img_output, "stdout"))
  {
    main_img_o = fopen(img_output, "w");
    if (!main_img_o)
    {
      perror("Erro ao abrir o arquivo de imagem!\n");
      exit(1);
    }
  }

  // Guardar imagem em main_img
  main_img = read_image(main_img_i);

  // Ler pastilhas
  initialize_imgarray(&tiles);
  read_tiles(&tiles, tile_dir);

  // Cria mosaico da imagem e a imprime
  make_mosaic(main_img, &tiles);
  print_image(main_img, main_img_o);
  
  // Desaloca todos os espaços da imagem
  destroy_img(main_img);
  destroy_imgarray(&tiles);

  // Fecha arquivos abertos
  if (strcmp(img_input, "stdin"))
    fclose(main_img_i);
  if (strcmp(img_output, "stdout"))
    fclose(main_img_o);

  return 0;
}
