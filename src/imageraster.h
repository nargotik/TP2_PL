#ifndef IMGRASTER_H
#define IMGRASTER_H
#define MAXLINE 1024

int *create_ppm_memory(int x, int y);
void imageFill(int *img_in, int x_size, int y_size, int r, int g, int b);

void writeImage(FILE *f, int *img_out, char *img_out_fname, int x_size, int y_size);
void readImageHeader(FILE *f, int *x_size, int *y_size);
void readImage(FILE *f, int *img_in);

#endif /* IMGRASTER_H */
