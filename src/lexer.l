%{
 #include "../src/commands.h"
 #include "../include/grammar.tab.h"
%}
/*11*/
%%

make        { return MAKE; }
repeat      { return REPEAT; }
forward|fd  { return FORWARD; }
right|rt    { return RIGHT; }

\"[a-z]+    { yylval.str = strdup(yytext+1); return VAR_NAME; }
:[a-z]+     { yylval.str = strdup(yytext+1); return VAR_VALUE; }

[0-9]+      { yylval.num = atoi(yytext); return INT; }

[ \t\n\r]   {}
.           { return yytext[0]; }

%%

int yywrap() { return 1; }
