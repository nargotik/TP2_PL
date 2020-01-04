/**
 * @file commands.h
 * @author
 *  - José Moura <a13742|at|alunos.ipca.pt>
 *  - Óscar Silva <a14383|at|alunos.ipca.pt>
 *  - Daniel Filipe <a17442|at|alunos.ipca.pt>
 * @date 01 Jan 2020
 * @brief
 * Ficheiro Header
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#define DEBUG 1
// Tabela de Simbolos
typedef struct _varList {
    char *var;
    float val;
    struct _varList* next;
} VarList;

typedef struct _Value {
    char *var;  // if this is null, use val, else use var name
    int val;
} Value;

typedef struct _Dimension {
    Value x;
    Value y;
} Dimension;

typedef struct _Point {
    Value x;
    Value y;
    struct _Point* next;
} Point;

typedef struct _Colour {
    Value r;
    Value g;
    Value b;
} Colour;

typedef struct _Command {
    int command;  // using grammar.tab.h enum (yytokentype)
    Point *point;
    Dimension *dimension;
    Colour *color;
    Value *val;
    Value *val2;
    Value *str;
    Value *str2;
    struct _Command *next;
    struct _Command *child;
} Command;


typedef struct _Image {
    int r,g,b;
    int x_size,y_size;
    VarList *vars;
    int *img_data;
} Image;

Image *newImage();


Colour *parseColour(Value *r, Value *g, Value *b);
Dimension *parseDimension(Value *x, Value *y);
Point *parsePoint(Point *lst, Value *x, Value *y);
Command* newCommand(int command, Point *pt, Dimension *dim, Colour *col, Value *val, Value *val2 ,Value *str, Value *str2, Command *child) ;
Value *parseValue(int val, char* str);

void runCommands(Command *lst, Image *image);
void Run(Command *lst);



Command* newVariable(char *variable, Value* value);
// Inserts at the head of a list a command
Command* insertCommand(Command *lst, Command *cmd);

void append(Point* list, Point* dataParam);
int gera_random(int min, int max);

#endif /* COMMANDS_H */
