%{
 #include "../src/commands.h"
 #include "../include/grammar.tab.h"
%}
/*11*/
%%

NEW|new     { return NEW; }
LOAD|load     { return LOAD; }
LINE|load     { return LINE; }
SAVE|save     { return SAVE; }
COLOR|color     { return COLOR; }
POINT|point     { return POINT; }
RECT|rect     { return RECT; }
RECTFILL|rectfill     { return RECTFILL; }
CIRC|circ     { return CIRC; }
POLYLINE|polyline     { return POLYLINE; }

\; { return EOC; }

\"[a-z]+    { yylval.str = strdup(yytext+1); return VAR_NAME; }
:[a-z]+     { yylval.str = strdup(yytext+1); return VAR_VALUE; }
\"[1-9a-zA-Z\.]+\"    { yylval.str = strdup(yytext+1); return FILE_NAME; }

[0-9]+      { yylval.num = atoi(yytext); return INT; }

[ \t\n\r]   {}
.           { return yytext[0]; }

%%

int yywrap() { return 1; }
