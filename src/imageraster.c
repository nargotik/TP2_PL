#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../src/imageraster.h"

/**
 *
 * @param a
 * @param b
 * @return
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 *
 * @param a
 * @param b
 * @return
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 *
 * @param img_in
 * @param x_size
 * @param y_size
 * @param x
 * @param y
 * @param r
 * @param g
 * @param b
 */
void drawPoint(int *img_in, int x_size, int y_size, int x, int y, int r, int g, int b) {
    if (x > x_size || y > x_size || x<=0 || y<=0)
        return;
    int imageindex = getArrayIndex(x,y,x_size,y_size);
    img_in[imageindex] = r;
    img_in[imageindex + 1] = g;
    img_in[imageindex + 2] = b;
}

/**
 *
 * @param img_in
 * @param x_size
 * @param y_size
 * @param raio
 * @param x
 * @param y
 * @param r
 * @param g
 * @param b
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
 *
 * @param img_in
 * @param x_size
 * @param y_size
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param c_r
 * @param c_g
 * @param c_b
 */
void drawLine(int *img_in, int x_size, int y_size,int x1, int y1 , int x2, int y2, int c_r, int c_g, int c_b) {
    //printf("%d > %d--",(x1-x2) , (y1-y2));
    if ((x1-x2) > (y1-y2)) {
        int xmin = min(x1,x2);
        int xmax = max(x1,x2);
        float m = (y2-y1)/(x2-x1);
        float b = y1 - m * x1;
        for (int x=xmin;x<= xmax;x++) {
            int y = m * x + b;
            drawPoint(img_in,x_size,y_size,x,y,c_r,c_g,c_b);
        };
    } else if ((x1-x2) <= (y1-y2)) {
        int ymin = min(y1,y2);
        int ymax = max(y1,y2);
        float m = (y2-y1)/(x2-x1);
        float b = y1 - m * x1;
        for (int y=ymin;y<= ymax;y++) {
            int x = (y-b)/m;
            drawPoint(img_in,x_size,y_size,x,y,c_r,c_g,c_b);
        }
    }
}

/**
 *
 * @param x coordenada a encontrar
 * @param y coordenada a encontrar
 * @param x_size tamanho em x da imagem
 * @param y_size tamanho em y da imagem
 * @return
 */
int getArrayIndex(int x, int y, int x_size, int y_size) {
    return (x + ((y - 1) * x_size)) * 3 - 3;
}

/**
 *
 * @param position
 * @param x_size
 * @param y_size
 * @param x
 * @param y
 */
void getImagePosition(int position, int x_size, int y_size ,int *x, int *y) {
    position = position / 3 ;
    *y = (position) / x_size + 1;
    *x = (position) % x_size + 1;
}

/**
 *
 * @param x
 * @param y
 * @return
 */
int *create_ppm_memory(int x, int y) {
    return malloc((3 * x * y) * sizeof(int));
}

/**
 *
 * @param img_in
 * @param x_size
 * @param y_size
 * @param r
 * @param g
 * @param b
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
 *
 * @param f
 * @param img_in
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
 *
 * @param f
 * @param x_size
 * @param y_size
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

  fprintf(stderr, "Info: magic=%s, x_val=%d, y_val=%d, maxcolors_val=%d\n",
    magic, x_val, y_val, maxcolors_val);
  *x_size   = x_val;
  *y_size   = y_val;
}

/**
 *
 * @param f
 * @param img_out
 * @param img_out_fname
 * @param x_size
 * @param y_size
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
