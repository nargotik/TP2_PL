/**
 * @file grammar.y
 * @author
 *  - José Moura <a13742|at|alunos.ipca.pt>
 *  - Óscar Silva <a14383|at|alunos.ipca.pt>
 *  - Daniel Filipe <a17442|at|alunos.ipca.pt>
 * @date 01 Jan 2020
 * @brief
 * Ficheiro de gramatica bison yacc
 */

%{
  #include <stdio.h>
  #include <string.h>
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

%token NEW LOAD SAVE COLOR POINT RECT RECTFILL CIRC
%token POLYLINE EOC LINE RAND CRUZ FOR IN DO END TO ATTRIB ATTRIB_RAND CIRCFILL INVERT
%token<str> VAR_VALUE VAR_NAME FILE_NAME
%token<num> INT

%type<cmd> instructionList instruction
%type<dim> v_dimension
%type<col> v_colour
%type<pt> v_point v_points_multiple v_points_two
%type<val> value

%start program;
%expect 1

%%

program : instructionList { Run($1); }
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

             | INVERT  EOC {
                              //printf("CMD NEW: %d x %d - COLOR %d,%d,%d\n" ,$2->x.val,$2->y.val,$3->r.val,$3->g.val,$3->b.val);
                              $$ =   newCommand(
                                INVERT, /* Command*/
                                NULL, /*Point*/
                                NULL,  /*Dimensions*/
                                NULL, /*Colours*/
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
                  char *filename = strdup($2);
                  filename[strlen(filename) - 1 ] = '\0';

                  $$ =   newCommand(
                    LOAD, /* Command*/
                    NULL, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    parseValue(0,filename), /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
            }
            | SAVE FILE_NAME EOC {
                  //printf("CMD Save: %s\n" ,$2);
                  char *filename = strdup($2);
                  filename[strlen(filename) - 1 ] = '\0';

                  $$ =   newCommand(
                    SAVE, /* Command*/
                    NULL, /*Point*/
                    NULL,  /*Dimensions*/
                    NULL, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    parseValue(0,filename), /*str1*/
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
                    RECT, /* Command */
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
            | CIRCFILL v_point value EOC {
                   //printf("CIRC WITH 1 POINT %d,%d  %d  %d:%d:%d\n" ,$2->x.val,$2->y.val,$3->val, $4->r.val, $4->g.val, $4->b.val);
                   $$ =   newCommand(
                     CIRCFILL, /* Command*/
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
            | CIRCFILL v_point value v_colour EOC {
                   //printf("CIRC WITH 1 POINT %d,%d  %d  %d:%d:%d\n" ,$2->x.val,$2->y.val,$3->val, $4->r.val, $4->g.val, $4->b.val);
                   $$ =   newCommand(
                     CIRCFILL, /* Command*/
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

                  $$ =   newCommand(
                    POLYLINE, /* Command*/
                    $3, /*Point*/
                    NULL,  /*Dimensions*/
                    $4, /*Colours*/
                    NULL, /*Val*/
                    NULL, /*Val1*/
                    NULL, /*str1*/
                    NULL, /*str2*/
                    NULL /*next*/
                    );
                    // Acrescenta o ponto no final da lista
                    while ($3->next != NULL) {
                        $3 = $3->next;
                    }
                    $3->next = $2;
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
                parseValue(0,$1), /*str1*/
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
                parseValue(0,$3), /*Val*/
                NULL, /*Val1*/
                parseValue(0,$1), /*str1*/
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
                $4, /*Val*/
                NULL, /*Val1*/
                parseValue(0,$1), /*str1*/
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
                parseValue(0,$2), /*str1*/
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
          $2->next = $1;
          $$ = $2;
      }
      ;
v_points_two : v_point v_point {
        $1->next = $2;
        $$ = $1;
};


value : INT  {  Value* v = (Value*)malloc(sizeof(Value));
               v->val = $1;
               v->var = NULL;
               $$ = v; }
     | VAR_NAME {
               Value *v = (Value*)malloc(sizeof(Value));
               v->var = $1;
               $$ = v; }
     ;


%%

int yyerror(char* msg) { printf("Error: %s\n", msg); return 0; }
