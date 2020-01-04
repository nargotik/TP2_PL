
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../src/imageraster.h"

/**
 * @brief
 * Devolve o minimo de dois inteiros
 * @param a inteiro 1
 * @param b inteiro 2
 * @return minimo ente a e b
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * @brief
 * Devolve o máximo de dois inteiros
 * @param a inteiro 1
 * @param b inteiro 2
 * @return máximo ente a e b
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * @brief
 * Desenha um pixel na imagem
 * @param img_in array da imagem
 * @param x_size tamanho da imagem em x
 * @param y_size tamanho da imagem em y
 * @param x ponto em x
 * @param y ponto em y
 * @param r cor vermelha
 * @param g cor verde
 * @param b cor azul
 */
void drawPoint(int *img_in, int x_size, int y_size, int x, int y, int r, int g, int b) {
    if (x > x_size || y > x_size || x<=0 || y<=0) {
        return;
    }

    int imageindex = getArrayIndex(x,y,x_size,y_size);
    img_in[imageindex] = r;
    img_in[imageindex + 1] = g;
    img_in[imageindex + 2] = b;
}

/**
 * @brief
 * Desenha um circulo com um raio desde o ponto x e y
 * @see drawPoint
 * @param img_in array dos pixels da imagem
 * @param x_size tamanho da imagem em x
 * @param y_size tamanho da imagem em y
 * @param raio raio do circulo
 * @param x coordenada x do centro
 * @param y coordenada y do centro
 * @param r cor vermelha
 * @param g cor verde
 * @param b cor azul
 */
void drawCircle(int *img_in, int x_size, int y_size,int raio,int x, int y, int r, int g, int b) {
    float step = 0.001;
    float theta = 0;
    for ( ; theta <= 2 * M_PI; theta += step) {
        int x_next = (int)(cos(theta) * raio + x);
        int y_next = (int)(sin(theta) * raio + x);
        drawPoint(img_in,x_size,y_size,x_next,y_next,r,g,b);
    }
}

/**
 * @brief
 * Desenha uma linha entre dois pontos x1,y1 x2,y2
 * @see drawPoint
 * @param img_in array dos pixels da imagem
 * @param x_size tamanho da imagem em x
 * @param y_size tamanho da imagem em y
 * @param x1 coordenada x1
 * @param y1 coordenada y1
 * @param x2 coordenada x2
 * @param y2 coordenada y2
 * @param c_r cor vermelha
 * @param c_g cor verde
 * @param c_b cor azul
 */
void drawLine(int *img_in, int x_size, int y_size,int x1, int y1 , int x2, int y2, int c_r, int c_g, int c_b) {
    printf("(%d-%d) > (%d-%d) %d>%d",x1,x2,y1,y2,x1-x2,y1-y2);
    // Vertical
    if (x1 == x2) {
        int ymin = min(y1,y2);
        int ymax = max(y1,y2);
        for (int y=ymin; y<=ymax; y++) {
            drawPoint(img_in,x_size,y_size,x1,y,c_r,c_g,c_b);
        }
    } else if (y1 == y2) {
        // Horizontal
        int xmin = min(x1,x2);
        int xmax = max(x1,x2);
        for (int x=xmin; x<=xmax; x++) {
            drawPoint(img_in,x_size,y_size,x,y1,c_r,c_g,c_b);
        }
    } else if (abs(x1-x2) > abs(y1-y2)) {
        int xmin = min(x1,x2);
        int xmax = max(x1,x2);
        float m = (y2-y1)/(x2-x1);
        float b = y1 - m * x1;
        for (float x=xmin;x<= xmax;x += 0.01) {
            float y = m * x + b;
            drawPoint(img_in,x_size,y_size,x,y,c_r,c_g,c_b);
        };
        printf("-1-");
    } else if (abs(x1-x2) <= abs(y1-y2)) {

        int ymin = min(y1,y2);
        int ymax = max(y1,y2);
        float m = (y2-y1)/(x2-x1);
        float b = y1 - m * x1;
        for (float y=ymin;y<= ymax;y += 0.01) {
            float x = (y-b)/m;
            drawPoint(img_in,x_size,y_size,x,y,c_r,c_g,c_b);
        }
        printf("-%d %d-2-", ymin,ymax);
    }
}

/**
 * @brief
 * Devolve o index no array para uma determinada coordenada.
 * @see getImagePosition
 * @param x coordenada a encontrar
 * @param y coordenada a encontrar
 * @param x_size tamanho em x da imagem
 * @param y_size tamanho em y da imagem
 * @return inteiro do indice no array correspondente
 */
int getArrayIndex(int x, int y, int x_size, int y_size) {
    return (x + ((y - 1) * x_size)) * 3 - 3;
}

/**
 * @brief
 * Devolve a coordenada x e y para um determinado posicao no array
 * @see getArrayIndex
 * @param position inteiro da posicao no array
 * @param x_size tamanho em x da imagem
 * @param y_size tamanho em y da imagem
 * @param x *POINTER - coordenada x devolvida por ref
 * @param y *POINTER - coordenada y devolvida por ref
 */
void getImagePosition(int position, int x_size, int y_size ,int *x, int *y) {
    position = position / 3 ;
    *y = (position) / x_size + 1;
    *x = (position) % x_size + 1;
}

/**
 * @brief
 * Preenche uma imagem completa com uma cor (utilizado para fazer backgrouds)
 * @param img_in img_in array dos pixels da imagem
 * @param x_size tamanho da imagem em x
 * @param y_size tamanho da imagem em y
 * @param r cor vermelha
 * @param g cor verde
 * @param b cor azul
 */
void imageFill(int *img_in, int x_size, int y_size, int r, int g, int b) {

    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            img_in[3*(i*x_size+j)+0] = r;
            img_in[3*(i*x_size+j)+1] = g;
            img_in[3*(i*x_size+j)+2] = b;
        }
    }
}

/**
 * @brief
 * Le a informação da imagem
 * Esta funcao deve ser executada depois da readImageHeader
 * @see readImageHeader
 * @param f file pointer
 * @param img_in img_in array dos pixels da imagem
 */
void readImage(FILE *f, int *img_in)
{
    int i=0, c;
    int r_val, g_val, b_val;

    while ((c = fgetc(f)) != EOF) {
        ungetc(c, f);

        r_val = fgetc(f);
        g_val = fgetc(f);
        b_val = fgetc(f);

        img_in[i++] = r_val;
        img_in[i++] = g_val;
        img_in[i++] = b_val;
    }
    fclose(f);
}

/**
 * @brief
 * Le o cabeçalho de uma imagem ficheiro e devolve o x_size e y_size
 * @param f file pointer
 * @param x_size *POINTER - tamanho da imagem em x
 * @param y_size *POINTER - tamanho da imagem em y
 */
void readImageHeader(FILE *f, int *x_size, int *y_size)
{
  int flag=0;
  int x_val, y_val, maxcolors_val;
  unsigned int i;
  char magic[MAXLINE];
  char line[MAXLINE];
  int count=0;

  /* Read the PPM file header. */
  while (fgets(line, MAXLINE, f) != NULL) {
    flag = 0;
    for (i = 0; i < strlen(line); i++) {
      if (isgraph(line[i]) && (flag == 0)) {
        if ((line[i] == '#') && (flag == 0)) {
          flag = 1;
        }
      }
    }
    if (flag == 0) {
      if (count == 0) {
        count += sscanf(line, "%s %d %d %d", magic, &x_val, &y_val, &maxcolors_val);
      } else if (count == 1) {
        count += sscanf(line, "%d %d %d", &x_val, &y_val, &maxcolors_val);
      } else if (count == 2) {
        count += sscanf(line, "%d %d", &y_val, &maxcolors_val);
      } else if (count == 3) {
        count += sscanf(line, "%d", &maxcolors_val);
      }
    }
    if (count == 4) {
      break;
    }
  }

  if (!(strcmp(magic, "P6") == 0)) {
    fprintf(stderr, "Error: Input file not in PPM format!\n");
    exit(1);
  }

  fprintf(stderr, "Read: type=%s, x_size=%d, y_size=%d, maxcolors_val=%d\n",
    magic, x_val, y_val, maxcolors_val);
  *x_size   = x_val;
  *y_size   = y_val;
}

/**
 * @brief
 * Grava num ficheiro uma imagem
 * @param f file pointer
 * @param img_out array dos pixels da imagem
 * @param img_out_fname nome que damos à imagem (usado para comentario)
 * @param x_size tamanho da imagem em x
 * @param y_size tamanho da imagem em y
 */
void writeImage(FILE *f, int *img_out, char *img_out_fname, int x_size, int y_size)
{
  int i, j;
  fprintf(f, "P6\n");

  fprintf(f, "# %s\n", img_out_fname);

  fprintf(f, "%d %d\n", x_size, y_size);

  fprintf(f, "%d\n", 255);

  /* Write the image data. */
  for (i = 0; i < y_size; i++) {
    for (j = 0; j < x_size; j++) {
        fprintf(f, "%c%c%c",
          img_out[3*(i*x_size+j)+0],
          img_out[3*(i*x_size+j)+1],
          img_out[3*(i*x_size+j)+2]);
    }
  }
  fclose(f);
}
