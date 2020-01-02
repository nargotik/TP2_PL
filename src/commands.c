#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../src/commands.h"
#include "../src/imageraster.h"
#include "../include/grammar.tab.h"

Colour *parseColour(Value *r, Value *g, Value *b) {
     Colour* v = (Colour*)malloc(sizeof(Colour));
     v->r = *r;
     v->g = *g;
     v->b = *b;
     return v;
}

Dimension *parseDimension(Value *x, Value *y) {
     Dimension* v = (Dimension*)malloc(sizeof(Dimension));
     v->x = *x;
     v->y = *y;
     return v;
}

Point *parsePoint(Point *lst, Value *x, Value *y) {
     Point* v = (Point*)malloc(sizeof(Point));
     v->x = *x;
     v->y = *y;
     v->next = lst;
     return v;
}

Value *parseValue(int val, char* str) {
    Value* v = (Value*)malloc(sizeof(Value));
     v->val = val;
     v->var = str; // so we know this is an INTeger
     return v;
}

Command* newCommand(
  int command,
  Point *pt,
  Dimension *dim,
  Colour *col,
  Value *val,
  Value *val2 ,
  Value *str,
  Value *str2,
  Command *child
) {
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

Command* insertCommand(Command *lst, Command *cmd) {
    cmd->next = lst;
    return cmd;
}

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

int evalValue(VarList *lst, Value *val) {

    if (val->var == NULL) {
        return val->val;
    }
    else {
        return evalVar(lst, val->var);
    }
}

Image *newImage() {
    Image *image = (Image*) malloc(sizeof(Image));
    image->r = 255;
    image->g = 255;
    image->b = 255;
    image->x_size = 1;
    image->y_size = 1;
    image->vars = NULL;  // No variables defined so far...
    return image;
}

void Run(Command *lst) {
    Image *image = newImage();
    runCommands(lst, image);
}


void runCommands(Command *lst, Image *image) {

    if (!lst) return;
    switch(lst->command) {
        case FOR:
            printf("repeat %d [\n", lst->val->val);
            runCommands(lst->child , image);
            printf("]\n");
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
            int r = (lst->color == NULL) ? 0 : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? 0 : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? 0 : evalValue(image->vars, &lst->color->b);

            // rgb cannot be bigger than 255
            r = (r>255) ? 255 : r;
            g = (g>255) ? 255 : g;
            b = (b>255) ? 255 : b;

            image->img_data = malloc((3 * image->x_size * image->y_size) * sizeof(int));

            // fill image with background color
            imageFill(image->img_data,image->x_size,image->y_size,r,g,b);
            fprintf(stderr,"\nNEW =>\n");
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
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("\nPOINT =>\n");
        }
            break;
        case LINE:
        {
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("LINE =>");
        }
            break;
        case RECT:
        {
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("RECT =>");
        }
            break;
        case RECTFILL:
        {
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("RECTFILL =>");
        }
            break;
        case CIRC:
        {
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("CIRC =>");
        };
            break;
        case POLYLINE:
        {
            // get rgb colour value, default def_r,def_g,def_b
            int r = (lst->color == NULL) ? image->r : evalValue(image->vars, &lst->color->r);
            int g = (lst->color == NULL) ? image->g : evalValue(image->vars, &lst->color->g);
            int b = (lst->color == NULL) ? image->b : evalValue(image->vars, &lst->color->b);
            printf("POLYLINE =>");
        }
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
                //printf("\n%s = %d\n", lst->str->var, evalValue(image->vars, lst->str));
            }
            break;
    }
    runCommands(lst->next, image);
}


int gera_random(int min, int max) {
    int numero_aleatorio;
    srand((unsigned) time(NULL));
    numero_aleatorio = rand() % max + min;
    return numero_aleatorio;
}


void DrawCommand(Command *lst, Turtle *turtle) {
    if (!lst) return; // no more commands to process

    /*switch (lst->command) {

        case MAKE:
           {
              // MAKE "var1 :var2
              // MAKE "var1 3243
              float value = evalValue(turtle->vars, lst->arg2);
              turtle->vars = updateVar(turtle->vars, lst->arg->var, value);
           }
           break;

        case RIGHT:
           {
               // RIGHT 50
               // RIGHT :var1
               float argVal = evalValue(turtle->vars, lst->arg);
               turtle->rot -= argVal * 2 * M_PI / 360;
           }
           break;

        case FORWARD:
           {
               // FORWARD 50
               // FORWARD :var1
               float argVal = evalValue(turtle->vars, lst->arg);
               float x2 =  cos(turtle->rot) * argVal + turtle->x;
               float y2 = -sin(turtle->rot) * argVal + turtle->y;
               printf(
                   "<line x1='%f' y1='%f' x2='%f' y2='%f' style='stroke:black'/>\n",
                   turtle->x, turtle->y, x2, y2);
               turtle->x = x2;
               turtle->y = y2;
           }
           break;

       case REPEAT:
           {
               // REPEAT 50 [ ... ]
               // REPEAT :var1 [ ... ]
               float argVal = evalValue(turtle->vars, lst->arg);
               for (int i = 0; i < argVal; i++)
                   DrawCommand(lst->child, turtle);
           }
           break;
    }

    DrawCommand(lst->next, turtle);
    */
}



