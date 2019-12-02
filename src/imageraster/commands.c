#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "grammar.tab.h"

Turtle *newTurtle() {
    Turtle *turtle = (Turtle*) malloc(sizeof(Turtle));
    turtle->x = turtle->y = 200;
    turtle->rot = M_PI / 2;
    turtle->vars = NULL;  // No variables defined so far...
    return turtle;
}

float evalVar(VarList *lst, char *varName) {
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

VarList* updateVar(VarList *lst, char *varName, float varValue) {
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

float evalValue(VarList *lst, Value *val) {
    if (val->var == NULL) {
        return val->val;
    }
    else {
        return evalVar(lst, val->var);
    }
}

void DrawCommand(Command *lst, Turtle *turtle) {
    if (!lst) return; // no more commands to process

    switch (lst->command) {

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
}

void Draw(Command *lst) {
    Turtle *turtle = newTurtle();
    printf("<?xml version=\"1.0\"?>\n");
    printf("<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    /// desenhar linhas
    DrawCommand(lst, turtle);
    printf("</svg>\n");
}

Command* newCommand(int command, Value *arg, Command *child) {
    Command *node = (Command*) malloc (sizeof(Command));
    node->command = command;
    node->arg = arg;
    node->child = child;
    node->next = NULL;
    return node;
}

Command* insertCommand(Command *lst, Command *cmd) {
    cmd->next = lst;
    return cmd;
}

Command* newVariable(char *variable, Value* value) {
    Command *cmd = (Command*) malloc(sizeof(Command));
    cmd->command = MAKE;
    cmd->arg = (Value*) malloc(sizeof(Value));
    cmd->arg->var = variable;
    cmd->arg2 = value;
    cmd->next = NULL;
    cmd->child = NULL;
    return cmd;
}

/*
void showCommands(Command *lst) {
    if (!lst) return;
    switch(lst->command) {
        case REPEAT: 
            printf("repeat %d [\n", lst->arg);
            showCommands(lst->child);
            printf("]\n");
            break;
        case RIGHT:
            printf("right %d\n", lst->arg);
            break;
        case FORWARD:
            printf("forward %d\n", lst->arg);
            break;
    }
    showCommands(lst->next);
}
*/