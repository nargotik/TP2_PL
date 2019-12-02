
// Tabela de Simbolos
typedef struct _varList {
    char *var;
    float val;
    struct _varList* next;
} VarList;

typedef struct _Turtle {
    float x, y, rot;
    VarList *vars;
} Turtle;

Turtle *newTurtle();

typedef struct _Value {
    // H1
    //  int type;
    //  union { char *var; int val; } value;
    // H2
    char *var;  // if this is null, use val, else use var name
    int val;
} Value;

typedef struct _Command {
    int command;  // using grammar.tab.h enum (yytokentype)
    Value *arg;
    Value *arg2;   // second argument, used on make
    struct _Command *next;
    struct _Command *child;
} Command;


void Draw(Command *lst);
// Creates a new cell for a command
Command* newCommand(int command, Value *arg, Command *child);
Command* newVariable(char *variable, Value* value);
// Inserts at the head of a list a command
Command* insertCommand(Command *lst, Command *cmd);
// Pretty print command list
void showCommands(Command *lst);
