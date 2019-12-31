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
FOR|for     { return FOR; }
IN|in     { return IN; }
DO|do     { return DO; }
END|end     { return END; }
\.\.    { return TO; }

RAND|rand     { return RAND; }

\; { return EOC; }

x { return CRUZ; }

[0-9]+      { yylval.num = atoi(yytext); return INT; }

[a-v]+([0-9]+)?    { yylval.str = strdup(yytext); return VAR_NAME; }

\"[1-9a-zA-Z\.]+\"    { yylval.str = strdup(yytext+1); return FILE_NAME; }



[ \t\n\r]   {}
.           { return yytext[0]; }

%%

int yywrap() { return 1; }
