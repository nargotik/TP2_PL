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
    Value *arg;
    Value *arg2;   // second argument, used on make
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

typedef struct _Turtle {
    float x, y, rot;
    VarList *vars;
} Turtle;

Turtle *newTurtle();

Colour *parseColour(Value *r, Value *g, Value *b);
Dimension *parseDimension(Value *x, Value *y);
Point *parsePoint(Point *lst, Value *x, Value *y);
Command* newCommand(int command, Point *pt, Dimension *dim, Colour *col, Value *val, Value *val2 ,Value *str, Value *str2, Command *child) ;
Value *parseValue(int val, char* str);

void Draw(Command *lst);
// Creates a new cell for a command

Command* newVariable(char *variable, Value* value);
// Inserts at the head of a list a command
Command* insertCommand(Command *lst, Command *cmd);
// Pretty print command list
void showCommands(Command *lst);
