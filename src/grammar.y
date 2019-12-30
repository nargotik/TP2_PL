%{
  #include <stdio.h>
  #include "../src/commands.h"
  int yylex();
  int yyerror(char*);
  #define DEBUG 1
%}
%union {
   int num;
   Command *cmd; // both a single command or a command list
   Value *val;
   Dimension *dim;
   Colour *col;
   Point *pt;
   char *str;  // used for variable names
}

%token NEW LOAD SAVE COLOR POINT RECT RECTFILL CIRC POLYLINE EOC LINE
%token<str> VAR_VALUE VAR_NAME FILE_NAME
%token<num> INT

%type<cmd> instructionList instruction
%type<dim> v_dimension
%type<col> v_colour
%type<pt> v_point v_points_multiple v_points_two
%type<val> value

%start instructionList;

%%

program : instructionList { Draw($1); }
        ;

instructionList : instruction { $$ = $1; }
                | instruction instructionList
                       { $$ = insertCommand($2,$1); }
                ;

instruction : NEW v_dimension EOC {
                  printf("CMD NEW: %d x %d \n" ,$2->x,$2->y);
            }
            | NEW v_dimension v_colour EOC {
                  printf("CMD NEW: %d x %d - COLOR %d,%d,%d\n" ,$2->x,$2->y,$3->r,$3->g,$3->b);
            }
            | COLOR v_colour EOC {
                  printf("CMD COLOR: %d,%d,%d\n" ,$2->r,$2->g,$2->b);
            }
            | LOAD FILE_NAME EOC {
                  printf("CMD Load: %s\n" ,$2);
            }
            | SAVE FILE_NAME EOC {
                  printf("CMD Save: %s\n" ,$2);
            }
            | POINT v_point EOC {
                  printf("POINT: %d,%d\n" ,$2->x,$2->y);
            }
            | POINT v_point v_colour EOC {
                  printf("POINT: %d,%d %d:%d:%d\n" ,$2->x,$2->y, $3->r, $3->g, $3->b);
            }
            | LINE v_points_two EOC {
                  printf("LINE: %d,%d %d,%d \n" ,$2->x,$2->y,$2->next->x,$2->next->y);
            }
            | LINE v_points_two v_colour EOC {
                  printf("LINE: %d,%d %d,%d %d:%d:%d\n" ,$2->x,$2->y,$2->next->x,$2->next->y, $3->r, $3->g, $3->b);
            }
            | RECT v_points_two EOC {
                  printf("RECT WITH 2 POINTS %d,%d %d,%d \n" ,$2->x,$2->y,$2->next->x,$2->next->y);
            }
            | RECT v_points_two v_colour EOC {
                  printf("RECT WITH 2 POINTS AND COLOR: %d,%d %d,%d %d:%d:%d\n" ,$2->x,$2->y,$2->next->x,$2->next->y, $3->r, $3->g, $3->b);
            }
            | RECT v_point v_dimension EOC {
                  printf("RECT WITH 1 POINT %d,%d  %d x %d\n" ,$2->x,$2->y,$3->x,$3->y);
            }
            | RECT v_point v_dimension v_colour EOC {
                  printf("RECT WITH 1 POINT %d,%d  %dx%d  %d:%d:%d\n" ,$2->x,$2->y,$3->x,$3->y, $4->r, $4->g, $4->b);
            } RECTFILL v_points_two EOC {
                  printf("RECT WITH 2 POINTS %d,%d %d,%d \n" ,$2->x,$2->y,$2->next->x,$2->next->y);
            }
            | RECTFILL v_points_two v_colour EOC {
                  printf("RECT WITH 2 POINTS AND COLOR: %d,%d %d,%d %d:%d:%d\n" ,$2->x,$2->y,$2->next->x,$2->next->y, $3->r, $3->g, $3->b);
            }
            | RECTFILL v_point v_dimension EOC {
                  printf("RECT WITH 1 POINT %d,%d  %d x %d\n" ,$2->x,$2->y,$3->x,$3->y);
            }
            | RECTFILL v_point v_dimension v_colour EOC {
                  printf("RECT WITH 1 POINT %d,%d  %dx%d  %d:%d:%d\n" ,$2->x,$2->y,$3->x,$3->y, $4->r, $4->g, $4->b);
            }
            | CIRC v_point value EOC {
                  printf("CIRC WITH 1 POINT %d,%d  %d\n",$2->x,$2->y,$3->val);
            }
            | CIRC v_point value v_colour EOC {
                  printf("CIRC WITH 1 POINT %d,%d  %d  %d:%d:%d\n" ,$2->x,$2->y,$3->val, $4->r, $4->g, $4->b);
            }
            | POLYLINE v_point v_points_multiple EOC {
                  printf("POLYLINE NO COLOR %d,%d  %d,%d\n" ,$2->x,$2->y,$3->x, $3->y);
            }
            | POLYLINE v_point v_points_multiple v_colour EOC {
                  printf("POLYLINE WITH COLOR %d,%d  %d,%d\n" ,$2->x,$2->y,$3->x, $3->y);
            }
            ;


v_dimension : value 'x' value {
                 $$ = parseDimension($1->val,$3->val);
};

v_colour  : value ':' value ':' value {
                $$ = parseColour($1->val, $3->val, $5->val);
};

v_point : value ',' value {
                $$ = parsePoint(NULL,$1->val,$3->val);
};

v_points_multiple : v_point {
          $$ = $1;
      }
      | v_points_multiple v_point {
          $1->next = $2;
          $$ = $1;
      }
      ;
v_points_two : v_point v_point {
        $1->next = $2;
        $$ = $1;
};


value : INT  {  Value* v = (Value*)malloc(sizeof(Value));
               v->val = $1;
               v->var = NULL; // so we know this is an INTeger
               $$ = v; }
     | VAR_VALUE {
               Value *v = (Value*)malloc(sizeof(Value));
               v->var = $1;
               $$ = v; }
     ;


%%

int yyerror(char* msg) { printf("Error: %s\n", msg); return 0; }
