#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "../src/imageraster.h"

// img_data = malloc((3 * x_dim * y_dim) * sizeof(int));

void writePoint(int *img_in, int x, int y, int r, int g, int b) {
    //img_in[3*(i*x_size+j)+0] = r;
    //img_in[3*(i*x_size+j)+1] = g;
    //img_in[3*(i*x_size+j)+2] = b;
}

int *create_ppm_memory(int x, int y) {
    return malloc((3 * x * y) * sizeof(int));
}

void imageFill(int *img_in, int x_size, int y_size, int r, int g, int b) {
    printf("Painting %d - %d - %d",r,g,b);
    for (int i = 0; i < y_size; i++) {
        for (int j = 0; j < x_size; j++) {
            img_in[3*(i*x_size+j)+0] = r;
            img_in[3*(i*x_size+j)+1] = g;
            img_in[3*(i*x_size+j)+2] = b;
        }
    }
}

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
