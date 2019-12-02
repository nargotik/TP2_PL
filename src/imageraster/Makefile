logo: main.o lex.yy.o grammar.tab.o commands.o
	gcc -o logo main.o lex.yy.o grammar.tab.o commands.o -lm

lex.yy.c: lexer.l
	flex lexer.l

grammar.tab.c grammar.tab.h: grammar.y
	bison -d grammar.y

grammar.tab.o: grammar.tab.c commands.h

main.o: main.c grammar.tab.h commands.h

lex.yy.o: lex.yy.c grammar.tab.h commands.h

commands.o: commands.c commands.h grammar.tab.h 



