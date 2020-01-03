#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/commands.h"
#include "../src/imageraster.h"
#include "../include/grammar.tab.h"

/**
 *
 * @param r
 * @param g
 * @param b
 * @return
 */
Colour *parseColour(Value *r, Value *g, Value *b) {
     Colour* v = (Colour*)malloc(sizeof(Colour));
     v->r = *r;
     v->g = *g;
     v->b = *b;
     return v;
}

/**
 *
 * @param x
 * @param y
 * @return
 */
Dimension *parseDimension(Value *x, Value *y) {
     Dimension* v = (Dimension*)malloc(sizeof(Dimension));
     v->x = *x;
     v->y = *y;
     return v;
}

/**
 *
 * @param lst
 * @param x
 * @param y
 * @return
 */
Point *parsePoint(Point *lst, Value *x, Value *y) {
     Point* v = (Point*)malloc(sizeof(Point));
     v->x = *x;
     v->y = *y;
     v->next = lst;
     return v;
}

/**
 *
 * @param val
 * @param str
 * @return
 */
Value *parseValue(int val, char* str) {
    Value* v = (Value*)malloc(sizeof(Value));
     v->val = val;
     v->var = str; // so we know this is an INTeger
     return v;
}

/**
 *
 * @param command
 * @param pt
 * @param dim
 * @param col
 * @param val
 * @param val2
 * @param str
 * @param str2
 * @param child
 * @return
 */
Command* newCommand(int command,Point *pt,Dimension *dim,Colour *col,Value *val,Value *val2 ,Value *str,Value *str2,Command *child)
{
    Command *node = (Command*) malloc (sizeof(Command));
    node->command = command;
    node->point = pt;
    node->dimension = dim;
    node->color = col;
    node->val = val;
    node->val2 = val2;
    node->str = str;
    node->str2 = str2;
    node->child = child;
    node->next = NULL;
    return node;
}

/**
 *
 * @param lst
 * @param cmd
 * @return
 */
Command* insertCommand(Command *lst, Command *cmd) {
    cmd->next = lst;
    return cmd;
}

/**
 *
 * @param lst
 * @param varName
 * @param varValue
 * @return
 */
VarList* updateVar(VarList *lst, char *varName, int varValue) {
    if (lst == NULL) {
        lst = (VarList*) malloc(sizeof(VarList));
        lst->next = NULL;
        lst->var = strdup(varName);
        lst->val = varValue;
    }
    else {
        if (strcmp(lst->var, varName) == 0) {
            lst->val = varValue;
        }
        else {
            lst->next = updateVar(lst->next, varName, varValue);
        }
    }
    return lst;
}

/**
 *
 * @param lst
 * @param varName
 * @return
 */
int evalVar(VarList *lst, char *varName) {
    if (lst == NULL) {
        fprintf(stderr, "Variable %s is undefined\n", varName);
        exit(1);
    }
    if (strcmp(lst->var, varName) == 0) {
        return lst->val;
    } else {
        return evalVar(lst->next, varName);
    }
}

/**
 *
 * @param lst
 * @param val
 * @return
 */
int evalValue(VarList *lst, Value *val) {

    if (val->var == NULL) {
        return val->val;
    }
    else {
        return evalVar(lst, val->var);
    }
}

/**
 *
 * @return
 */
Image *newImage() {
    Image *image = (Image*) malloc(sizeof(Image));
    image->r = 0;
    image->g = 0;
    image->b = 0;
    image->x_size = 1;
    image->y_size = 1;
    image->vars = NULL;  // No variables defined so far...
    return image;
}

/**
 *
 * @param lst
 */
void Run(Command *lst) {
    Image *image = newImage();
    runCommands(lst, image);
}

/**
 *
 * @param lst
 * @param image
 */
void runCommands(Command *lst, Image *image) {

    if (!lst) return;
    switch(lst->command) {
        case FOR:
            {
                // @done
                int from = evalValue(image->vars, lst->val);
                int to   = evalValue(image->vars, lst->val2);
                for (int var=from; var<=to ;var++) {
                    image->vars = updateVar(image->vars, lst->str->var, var);
                    runCommands(lst->child , image);
                }
            }
            break;
        case NEW:
        {
            // @done
            image->x_size = evalValue(image->vars, &lst->dimension->x);
            image->y_size = evalValue(image->vars, &lst->dimension->y);

            if(image->x_size<=0 || image->y_size<=0) {
                fprintf(stderr, "X_SIZE or Y_SIZE incorrect value\n");
                exit(1);
            }

            // get rgb background value, default 0,0,0
            int r = (lst->color == NULL) ? 255 : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? 255 : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? 255 : evalValue(image->vars, &lst->color->b);

            // rgb cannot be bigger than 255
            r = (r>255) ? 255 : r;
            g = (g>255) ? 255 : g;
            b = (b>255) ? 255 : b;

            image->img_data = malloc((3 * image->x_size * image->y_size) * sizeof(int));

            // fill image with background color
            imageFill(image->img_data,image->x_size,image->y_size,r,g,b);
            // fprintf(stderr,"\nNEW =>\n");
        }
            break;
        case COLOR:
        {
            // @done
            // get rgb colour value, default 0,0,0
            image->r = evalValue(image->vars, &lst->color->r);
            image->g = evalValue(image->vars, &lst->color->g);
            image->b = evalValue(image->vars, &lst->color->b);

            // rgb cannot be bigger than 255
            image->r = (image->r > 255) ? 255 : image->r;
            image->g = (image->g > 255) ? 255 : image->g;
            image->b = (image->b > 255) ? 255 : image->b;

            //printf("COLOR CHANGE - %d\\%d\\%d =>\n", def_r,def_g,def_b);
        }
            break;
        case LOAD:
        {
            // @done
            FILE *fp = fopen(lst->str->var, "r");

            fprintf(stderr,"\nReaded File %s", lst->str->var);

            readImageHeader(fp,&image->x_size,&image->y_size);

            image->img_data = malloc((3 * image->x_size * image->y_size) * sizeof(int));

            readImage(fp,image->img_data);

            fprintf(stderr,"[OK]\n");
        }
            break;
        case SAVE:
        {
            // @done
            FILE *fp = fopen(lst->str->var, "w");

            fprintf(stderr,"\nSaved File %s", lst->str->var);

            // Save image to file
            writeImage(fp,image->img_data,lst->str->var, image->x_size,image->y_size);
            fprintf(stderr,"[OK]\n");
        }
            break;
        case POINT:
        {
            // @done

            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);

            int x = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->x);
            int y = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->y);

            drawPoint(image->img_data,image->x_size,image->y_size,x,y,r,g,b);

        }
            break;
        case LINE:
        {
            // @done
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);

            int x1 = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->x);
            int y1 = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->y);

            int x2 = (lst->point->next == NULL) ? 1 : evalValue(image->vars, &lst->point->next->x);
            int y2 = (lst->point->next == NULL) ? 1 : evalValue(image->vars, &lst->point->next->y);


            drawLine(image->img_data,image->x_size,image->y_size,x1,y1,x2,y2,r,g,b);

        }
            break;
        case RECT:
        {
            // @done
            // get rgb colour value, default def_r,def_g,def_b

            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);


            int x1 = evalValue(image->vars, &lst->point->x);
            int y1 = evalValue(image->vars, &lst->point->y);

            int x2 = (lst->dimension == NULL) ?
                    evalValue(image->vars, &lst->point->next->x)
                    : x1 + evalValue(image->vars, &lst->dimension->x) - 1
                    ;
            int y2 = (lst->dimension == NULL) ?
                    evalValue(image->vars, &lst->point->next->y)
                    : y1 + evalValue(image->vars, &lst->dimension->y) - 1
                    ;
            //printf("%d %d %d %d %d %d %d xysize %d %d----",x1,y1,x2,y2,r,g,b,image->x_size,image->y_size);
            //exit(1);
            // Parte superior
            drawLine(image->img_data,image->x_size,image->y_size,x2,y1,x1,y1,r,g,b);

            // Lateral direita
            drawLine(image->img_data,image->x_size,image->y_size,x2,y2,x2,y1,r,g,b);

            // Baixo
            drawLine(image->img_data,image->x_size,image->y_size,x2,y2,x1,y2,r,g,b);

            // Lateral Equerda
            drawLine(image->img_data,image->x_size,image->y_size,x1,y2,x1,y1,r,g,b);

        }
            break;
        case RECTFILL:
        {
            // @done
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);

            int x1 = evalValue(image->vars, &lst->point->x);
            int y1 = evalValue(image->vars, &lst->point->y);

            int x2 = (lst->dimension == NULL) ?
                     evalValue(image->vars, &lst->point->next->x)
                                              : x1 + evalValue(image->vars, &lst->dimension->x) -1
            ;
            int y2 = (lst->dimension == NULL) ?
                     evalValue(image->vars, &lst->point->next->y)
                                              : y1 + evalValue(image->vars, &lst->dimension->y)-1
            ;

            for (int i=y1; i<= y2; i++) {
                // Parte superior
                drawLine(image->img_data,image->x_size,image->y_size,x1,i,x2,i,r,g,b);
            }
        }
            break;
        case POLYLINE:
        {
            // @todo

            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("POLYLINE =>");
        }
            break;
        case CIRC:
        {
            // @done
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);

            int x = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->x);
            int y = (lst->point == NULL) ? 1 : evalValue(image->vars, &lst->point->y);

            int raio = (lst->val == NULL) ? 1 : evalValue(image->vars, lst->val);

            drawCircle(image->img_data,image->x_size,image->y_size, raio,x,y,r,g,b);

        };
            break;
        case ATTRIB:
            {
                // @done
                // get value of variable or value
                int value = evalValue(image->vars, lst->val);
                // update variable
                image->vars = updateVar(image->vars, lst->str->var, value);
            }
            break;
        case ATTRIB_RAND:
            {
                // @done
                int value = evalValue(image->vars, lst->val);
                image->vars = updateVar(image->vars, lst->str->var, gera_random(0,value));
                printf("\n%s = %d . %d\n", lst->str->var, evalValue(image->vars, lst->str), gera_random(0,value));
            }
            break;
    }
    runCommands(lst->next, image);
}

/**
 *
 * @param min
 * @param max
 * @return
 */
int gera_random(int min, int max) {
    srand(time(0));
    return rand() % max + min;
}






