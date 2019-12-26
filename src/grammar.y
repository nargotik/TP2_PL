%{
  #include <stdio.h>
  #include "../src/commands.h"
  int yylex();
  int yyerror(char*);
%}
%union {
   int num;
   Command *cmd; // both a single command or a command list
   Value *val;
   char *str;  // used for variable names
}
%token FORWARD RIGHT REPEAT MAKE
%token<str> VAR_VALUE VAR_NAME
%token<num> INT
%type<cmd> instructionList instruction
%type<val> value

/*
%left '*' '/'
%left '+' '-'
tetes
*/

%%

program : instructionList { Draw($1); }
        ;

instructionList : instruction   { $$ = $1; }
                | instruction instructionList
                       { $$ = insertCommand($2,$1); }
                ;

instruction : MAKE VAR_NAME value { $$ = newVariable($2, $3); }
             /* MAKE VAR_NAME expr {
                              $$ = newVariable($2, NULL); // FIXME THIS IS WRONG!!!
                               } */
            | FORWARD value   { $$ = newCommand(FORWARD, $2, NULL); }
            | RIGHT value     { $$ = newCommand(RIGHT  , $2, NULL); }
            | REPEAT value '[' instructionList ']'  {
                              $$ = newCommand(REPEAT , $2, $4); }
            ;

value : INT  {  Value* v = (Value*)malloc(sizeof(Value));
                v->val = $1;
                v->var = NULL; // so we know this is an INTeger
                $$ = v; }
      | VAR_VALUE {
                Value *v = (Value*)malloc(sizeof(Value));
                v->var = $1;
                $$ = v; }
      ;

/*
expr : expr '+' expr
     | expr '-' expr
     | expr '/' expr
     | expr '*' expr
     | '(' expr ')'
     | value
     ;
*/

%%

int yyerror(char* msg) { printf("Error: %s\n", msg); return 0; }
