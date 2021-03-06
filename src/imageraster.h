/**
 * @file imageraster.h
 * @author
 *  - José Moura <a13742|at|alunos.ipca.pt>
 *  - Óscar Silva <a14383|at|alunos.ipca.pt>
 *  - Daniel Filipe <a17442|at|alunos.ipca.pt>
 * @date 01 Jan 2020
 * @brief
 * Contem as instruções necessárias para manipular imagem PNM
 */


#ifndef IMGRASTER_H
#define IMGRASTER_H
#define MAXLINE 1024

/*
 * Assinaturas das funções
 */

void imageFill(int *img_in, int x_size, int y_size, int r, int g, int b);
void writeImage(FILE *f, int *img_out, char *img_out_fname, int x_size, int y_size);
void readImageHeader(FILE *f, int *x_size, int *y_size);
void readImage(FILE *f, int *img_in);
int getArrayIndex(int x, int y, int x_size, int y_size);
void getImagePosition(int position, int x_size, int y_size ,int *x, int *y);
void drawPoint(int *img_in, int x_size, int y_size, int x, int y, int r, int g, int b);
void drawCircle(int *img_in, int x_size, int y_size,int raio,int x, int y, int r, int g, int b);
int max(int a, int b);
int min(int a, int b);
void drawLine(int *img_in, int x_size, int y_size,int x1, int y1 , int x2, int y2, int c_r, int c_g, int c_b);

#endif /* IMGRASTER_H */
