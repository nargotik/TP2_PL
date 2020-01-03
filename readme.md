# Trabalho Prático de PL

O presente software foi desenvolvido no contexto académico e deve ser utilizado sem qualquer garantia por conta e risco do utilizador.

[![Build Status](https://travis-ci.com/nargotik/TP2_PL.svg?token=qNfqiYyxNzmWoPqpyHnZ&branch=master)](https://travis-ci.com/nargotik/TP2_PL)


**Autores:**
- José Moura <a13742@alunos.ipca.pt>
- Óscar Silva <a14383@alunos.ipca.pt>
- Daniel Torres <a17442@alunos.ipca.pt>

**Links Uteis**
- GitHub: [https://github.com/nargotik/TP2_PL](https://github.com/nargotik/TP2_PL)
- Documentação: [https://nargotik.github.io/TP2_PL/](https://nargotik.github.io/TP2_PL/)


## Introdução

O objetivo deste trabalho é o de implementar uma ferramenta capaz de ler um documento de texto usando uma determinada sintaxe.
A resolução deste trabalho foi dividida em duas partes : 
- **Análise Léxica** para verificar se os caracteres introduzidos fazem parte do alfabeto por nós criado;
- **Análise Sintática** para verificar se os dados introduzidos seguem as regras gramaticais definidas na gramática por nós criada.

Para a realização deste trabalho foram utilizadas as ferramentas abaixo descritas:

- Flex (Versão 2.6.0)
- Bison (Versão 3.0.4)


## Utilização/Compilação
Para compilar as aplicações necessárias simplesmente é necessário efectuar o comando:
```shell script
$ make
```

## Estrutura do flex

O flex é um software desenvolvido com  o intuito de permitir a análise léxica de um determinado conjunto de dados através do reconhecimento de sequências de caracteres ("tokens") definidas pelo programador.

Um  ficheiro de bison é composto por três zonas delimitadas por **%%**

A primeira parte, o preâmbulo, é o local onde se faz a inclusão das bibliotecas e da definição de conjuntos de caracteres.

```c
%{
 #include "../src/commands.h"
 #include "../include/grammar.tab.h"
%}
``` 
No ficheiro por nós desenvolvido, o preâmbulo é constituído pela inclusão da biblioteca com a definição das funções necessárias para a criação de um software de desenho de imagens em Raster.
É também definida no preâmbulo o ficheiro grammar.tab.h que informa ao analisador léxico que o Flex vai ser usado em conjunto com o Bison.

A segunda parte, o corpo, é a zona na qual estão definidas as expressões regulares, assim como as ações semânticas que poderão ser despoletadas aquando do eventual reconhecimento de um *token*.

```flex
%%

NEW|new     { return NEW; }
LOAD|load     { return LOAD; }
LINE|line     { return LINE; }
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
```
Na nossa implementação, o corpo é constituído por expressões regulares que permitem reconhecer em *Ignore Case Sensitive* as diversas instruções e caracteres especiais utilizados pela nossa implementação de desenho de imagens em Rasper.


A terceira parte é destinada para as funções adicionais necessárias para o bom funcionamento do analisador léxico.

```c
int yywrap() { return 1; }
```
Nesta implementação, foi definida a função **yywrap()** que retorna 1 e que passa a informação ao analisador léxico que estamos na presença de EOF (End Of File).

## Estrutura do Bison

@todo

## Conclusão

Neste trabalho, surgiu uma necessidade acrescida ao nível de conhecimento abordado noutras cadeiras para a implementação de uma solução conforme pedido no enunciado. Como referido, a cadeira de SOSD foi uma ferramenta importante na implementação de comandos usados tais como o interpretador. Contudo com ajuda do flex, bison e de uma biblioteca com funções necessárias para a implementação do software de desenho com imagens em Raster, foram decisivos para o resultado final. Apesar dos inúmeros formatos pedidos, toda esta gestão só foi possível com uma estrutura bem delineada a nível de código em C, para que os ficheiros sejam gerados corretamente sem nenhuma perda de dados.
Em suma, pensamos ter cumprido com rigor aquilo a que este trabalho nos implicou ao nivel de conhecimento teórico e prático na disciplina de processamento de linguagens.

## Bibliografia / Referências

- [GitHub Flex](https://github.com/westes/flex)
- [Bison Lecture](http://web.eecs.utk.edu/~bvanderz/teaching/cs461Sp11/notes/bison/)
