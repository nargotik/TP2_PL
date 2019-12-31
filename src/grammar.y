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

%token NEW LOAD SAVE COLOR POINT RECT RECTFILL CIRC POLYLINE EOC LINE RAND CRUZ FOR IN DO END TO ATTRIB ATTRIB_RAND
%token<str> VAR_VALUE VAR_NAME FILE_NAME
%token<num> INT

%type<cmd> instructionList instruction
%type<dim> v_dimension
%type<col> v_colour
%type<pt> v_point v_points_multiple v_points_two
%type<val> value

%start program;

%%

program : instructionList { showCommands($1); }
        ;

instructionList : instruction { $$ = $1; }
                | instruction instructionList
                       { $$ = insertCommand($2,$1); }
                ;

// newCommand(int command, Point *pt, Dimension *dim, Colour *col, int val ,char *str, Command *child)
instruction : NEW v_dimension EOC {
                  //printf("CMD NEW: %d x %d \n" ,$2->x.val,$2->y.val);
                  $$ =   newCommand(
                    NEW, /* Command*/
                    NULL, /*Point*/
                    $2,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | NEW v_dimension v_colour EOC {
                  //printf("CMD NEW: %d x %d - COLOR %d,%d,%d\n" ,$2->x.val,$2->y.val,$3->r.val,$3->g.val,$3->b.val);
                  $$ =   newCommand(
                    NEW, /* Command*/
                    NULL, /*Point*/
                    $2,  /*Dimensions*/
                    $3, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | COLOR v_colour EOC {
                  //printf("CMD COLOR: %d,%d,%d\n" ,$2->r.val,$2->g.val,$2->b.val);
                  $$ =   newCommand(
                    COLOR, /* Command*/
                    NULL, /*Point*/
                    NULL,  /*Dimensions*/
                    $2, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | LOAD FILE_NAME EOC {
                  //printf("CMD Load: %s\n" ,$2);
                  $$ =   newCommand(
                    LOAD, /* Command*/
                    NULL, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/  //@todo $2
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | SAVE FILE_NAME EOC {
                  //printf("CMD Save: %s\n" ,$2);
                  $$ =   newCommand(
                    SAVE, /* Command*/
                    NULL, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/  //@todo $2
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | POINT v_point EOC {
                  //printf("POINT: %d,%d\n" ,$2->x.val,$2->y.val);
                  $$ =   newCommand(
                    POINT, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | POINT v_point v_colour EOC {
                  //printf("POINT: %d,%d %d:%d:%d\n" ,$2->x.val,$2->y.val, $3->r.val, $3->g.val, $3->b.val);
                  $$ =   newCommand(
                    POINT, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $3, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | LINE v_points_two EOC {
                  //printf("LINE: %d,%d %d,%d \n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val);
                  $$ =   newCommand(
                    LINE, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | LINE v_points_two v_colour EOC {
                  //printf("LINE: %d,%d %d,%d %d:%d:%d\n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val, $3->r.val, $3->g.val, $3->b.val);
                  $$ =   newCommand(
                    LINE, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $3, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECT v_points_two EOC {
                  //printf("RECT WITH 2 POINTS %d,%d %d,%d \n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val);
                  $$ =   newCommand(
                    RECT, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECT v_points_two v_colour EOC {
                  //printf("RECT WITH 2 POINTS AND COLOR: %d,%d %d,%d %d:%d:%d\n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val, $3->r.val, $3->g.val, $3->b.val);
                  $$ =   newCommand(
                    RECT, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $3, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECT v_point v_dimension EOC {
                  //printf("RECT WITH 1 POINT %d,%d  %d x %d\n" ,$2->x.val,$2->y.val,$3->x.val,$3->y.val);
                  $$ =   newCommand(
                    RECT, /* Command*/
                    $2, /*Point*/
                    $3,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECT v_point v_dimension v_colour EOC {
                  //printf("RECT WITH 1 POINT %d,%d  %dx%d  %d:%d:%d\n" ,$2->x.val,$2->y.val,$3->x.val,$3->y.val, $4->r.val, $4->g.val, $4->b.val);
                  $$ =   newCommand(
                    RECT, /* Command*/
                    $2, /*Point*/
                    $3,  /*Dimensions*/
                    $4, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECTFILL v_points_two EOC {
                  //printf("RECT WITH 2 POINTS %d,%d %d,%d \n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val);
                  $$ =   newCommand(
                    RECTFILL, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECTFILL v_points_two v_colour EOC {
                  //printf("RECT WITH 2 POINTS AND COLOR: %d,%d %d,%d %d:%d:%d\n" ,$2->x.val,$2->y.val,$2->next->x.val,$2->next->y.val, $3->r.val, $3->g.val, $3->b.val);
                  $$ =   newCommand(
                    RECTFILL, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $3, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECTFILL v_point v_dimension EOC {
                  //printf("RECT WITH 1 POINT %d,%d  %d x %d\n" ,$2->x.val,$2->y.val,$3->x.val,$3->y.val);
                  $$ =   newCommand(
                    RECTFILL, /* Command*/
                    $2, /*Point*/
                    $3,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | RECTFILL v_point v_dimension v_colour EOC {
                  //printf("RECT WITH 1 POINT %d,%d  %dx%d  %d:%d:%d\n" ,$2->x.val,$2->y.val,$3->x.val,$3->y.val, $4->r.val, $4->g.val, $4->b.val);
                  $$ =   newCommand(
                    RECTFILL, /* Command*/
                    $2, /*Point*/
                    $3,  /*Dimensions*/
                    $4, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | CIRC v_point value EOC {
                  //printf("CIRC WITH 1 POINT %d,%d  %d\n",$2->x.val,$2->y.val,$3->val);
                  $$ =   newCommand(
                    CIRC, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    $3, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | CIRC v_point value v_colour EOC {
                  //printf("CIRC WITH 1 POINT %d,%d  %d  %d:%d:%d\n" ,$2->x.val,$2->y.val,$3->val, $4->r.val, $4->g.val, $4->b.val);
                  $$ =   newCommand(
                    CIRC, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $4, /*Colours*/
                    $3, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | POLYLINE v_point v_points_multiple EOC {
                  //printf("POLYLINE NO COLOR %d,%d  %d,%d\n" ,$2->x.val,$2->y.val,$3->x.val, $3->y.val);
                  $2->next = $3;
                  $$ =   newCommand(
                    POLYLINE, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | POLYLINE v_point v_points_multiple v_colour EOC {
                //  printf("POLYLINE WITH COLOR %d,%d  %d,%d\n" ,$2->x.val,$2->y.val,$3->x.val, $3->y.val);
                  $2->next = $3;
                  $$ =   newCommand(
                    POLYLINE, /* Command*/
                    $2, /*Point*/
                    NULL,  /*Dimensions*/
                    $4, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | VAR_NAME '=' value EOC {
              // COMMAND ATTRIB
              //$$ = newVariable($2, $3);
              //printf("ATTRIB %s = %d\n" ,$1,$3->val);
              $$ =   newCommand(
                ATTRIB, /* Command*/
                NULL, /*Point*/
                NULL,  /*Dimensions*/
                NULL, /*Colours*/
                $3, /*Val*/
                NULL, /*Val1*/
                parseValue(0,$1), /*str1*/ //@todo $1
                NULL, /*str2*/
                NULL /*next*/
                );
            }
            | VAR_NAME '=' VAR_NAME EOC {
              // COMMAND ATTRIB
              //$$ = newVariable($2, $3);
              //printf("ATTRIB %s = %s\n" ,$1,$1);
              $$ =   newCommand(
                ATTRIB, /* Command*/
                NULL, /*Point*/
                NULL,  /*Dimensions*/
                NULL, /*Colours*/
                parseValue(0,$3), /*Val*/ //@todo $3
                NULL, /*Val1*/
                parseValue(0,$1), /*str1*/ //@todo $1
                NULL, /*str2*/
                NULL /*next*/
                );
            }
            | VAR_NAME '=' RAND value EOC {
              // COMMAND ATTRIB
              //printf("ATTRIB %s = RAND %d\n" ,$1,$4->val);
              $$ =   newCommand(
                ATTRIB_RAND, /* Command*/
                NULL, /*Point*/
                NULL,  /*Dimensions*/
                NULL, /*Colours*/
                $4, /*Val*/ //@todo $4
                NULL, /*Val1*/
                parseValue(0,$1), /*str1*/ //@todo $1
                NULL, /*str2*/
                NULL /*next*/
                );
            }
            | FOR VAR_NAME IN '[' value TO value ']' DO instructionList END FOR EOC {
              //printf("FOR %s %d .. %d %d\n" ,$2,$5->val,$7->val,$10->command);
              $$ =   newCommand(
                FOR, /* Command*/
                NULL, /*Point*/
                NULL,  /*Dimensions*/
                NULL, /*Colours*/
                $5, /*Val*/
                $7, /*Val1*/
                parseValue(0,$2), /*str1*/ //@todo $2
                NULL, /*str2*/
                $10 /*next*/
                );
            }
            ;


v_dimension : value CRUZ value {
                $$ = parseDimension($1,$3);
};

v_colour  : value ':' value ':' value {
                $$ = parseColour($1, $3, $5);
};

v_point : value ',' value {
                $$ = parsePoint(NULL,$1,$3);
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
     | VAR_NAME {
       printf("vat %s", $1);
               Value *v = (Value*)malloc(sizeof(Value));
               v->var = $1;
               $$ = v; }
     ;


%%

int yyerror(char* msg) { printf("Error: %s\n", msg); return 0; }
