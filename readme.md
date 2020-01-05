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

De forma  a comprovar os conceitos de analisador léxico e sintático, foi-nos proposta uma série de problemas e de entre essas propostas dadas, deveríamos escolher uma para implementar.
A nossa escolha recaiu no problema 2: Linguagem de Desenho de Imagens Raster por nos parecer um enunciado bastante interessante do ponto de vista de comprovar as valências do uso do "par Flex-Bison".
 
 De forma bastante sucinta, este problema propunha a implementação de uma linguagem para a descrição e criação de imagens através da interpretação de um simples documento de texto com determinada sintaxe pré-estabelecida.
 
Para a realização deste trabalho foram utilizadas as ferramentas abaixo descritas:

- Flex (Versão 2.6.0)
- Bison (Versão 3.0.4)

### Estratégia de resolução do Problema

 Para resolver tal problema foi-nos sugerido o uso do formato *PNM* por ser um formato bastante simples a nível de leitura e escrita e com a vantagem de poder ser armazenado em memória usando matrizes.
 A família *PNM* possui vários sub-formatos sendo designados por P*n* estando *n* definido entre 1 e 6. 
 As diferenças entre eles baseiam-se na forma como são codificados (ASCII ou Binário) e quais os conjuntos de cores que conseguem armazenar (B&W, 256 cinzentos ou 16M cores).
 
|Nome|P1  |P2  |P3  |P4  |P5  |P6  |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|Cores  |B&W  |256 Cinzentos  |16M Cores  |B&W  |256 Cinzentos  | 16M Cores |
|Codificação  |ASCII  |ASCII  |ASCII|Binário  |Binário  |Binário  |

 Face à representação vista na tabela acima, resolvemos utilizar o formato *P6* pois o mesmo permite a criação de imagens a 16M Cores, mesmo que a nível de codificação, os pixeis da imagem sejam em binário, usando um byte para representar cada cor.
 
 #### Definição da linguagem de Desenho
 
A linguagem de desenho é definida através da interpretação de um conjunto de comandos previamente estabelecidos e devem terminal por ponto e virgula (;).
Definiu-se ainda que:
- Os comandos deveriam ser escritos em maiúsculas;
-  As variáveis deveriam ser escritas em minúsculas;
-  Só se podem usar valores inteiros;
-  Um ponto geométrico é definido por dois inteiros separados por uma vírgula; 
- Uma área é definida por dois inteiros separados por 'x';
- Uma cor é definida através da representação numérica RGB (três inteiros separados por dois pontos);
- Uma string é delimitada por aspas simples.

#### 1. Gestão de Imagens

Para a gestão das imagens foram definidas os seguintes comandos:
- NEW - Comando que prepara a área de desenho, recebendo como parâmetros o tamanho da área de desenho (p.ex. 800x600) e a cor iniciada (p.ex. 255:0:0). 
```
			NEW 800x600 255:0:0;
```
O parâmetro cor é opcional, sendo que, no caso de ser omitido, a área de desenho é iniciada com a cor branca (255:255:255)
```
			NEW 800x600;
```
- LOAD - Comando encarregado de carregar um ficheiro *.pnm* para memória.
```
			LOAD "ficheiro.pnm";
```
- SAVE - Comando encarregado de guardar uma imagem gerada em memória num ficheiro de imagem P6.
```
			SAVE "ficheiro.pnm";
```
#### 2. Desenho

Segundo o enunciado, todas as primitivas de desenho deverão aceitar um último parâmetro com a cor a ser usada para desenhar. No entanto pode ser definida uma cor "por omissão", sendo que essa será definida pelo comando COLOR.

- COLOR - Comando responsável por definir a cor padrão caso os comandos não possuam uma cor definida nos seus parâmetros.
```
			COLOR 128:128:128;
```
- POINT - 
## Utilização/Compilação
Para compilar as aplicações necessárias simplesmente é necessário efectuar o comando:
```shell script
$ make
```

## Estrutura do flex

O flex é um software desenvolvido com  o intuito de permitir a análise léxica de um determinado conjunto de dados através do reconhecimento de sequências de caracteres ("tokens") definidas pelo programador.

Um  ficheiro de Flex é composto por três zonas delimitadas por **%%**

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

```c
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
Se for reconhecido algum dos *tokens* definidos no **lexer.l**, é retornado para o programa a devida função de C a ser invocada para tratar aquele comando.

A terceira parte é destinada para as funções adicionais necessárias para o bom funcionamento do analisador léxico.

```c
int yywrap() { return 1; }
```
Nesta implementação, foi definida a função **yywrap()** que retorna 1 e que passa a informação ao analisador léxico que estamos na presença de EOF (End Of File).

## Estrutura do Bison

O Bison é um software desenvolvido com  o intuito de permitir gerar automaticamente programas para análise sintática.
Possui como entrada a declaração de uma gramática, que especifica uma linguagem e gera como saída o *parser* dessa linguagem.
O *parser* gerado pelo Bison permite validar os argumentos lá introduzidos permitindo assim, averiguar se os mesmos seguem a lógica sintática definida na nossa gramática.
 
 @todo - Fazer o similar do Flex.
 
## Conclusão

@todo - Refazer Conclusão:
_ Adicionar imagens exemplificativas de como funciona a solução (pedir ao Daniel para gerar no Mint)
_ Vantagens e Desvantagens da implementação do "par Flex-Bison" (?)

[Neste trabalho, surgiu uma necessidade acrescida ao nível de conhecimento abordado noutras cadeiras para a implementação de uma solução conforme pedido no enunciado. Como referido, a cadeira de SOSD foi uma ferramenta importante na implementação de comandos usados tais como o interpretador. Contudo com ajuda do flex, bison e de uma biblioteca com funções necessárias para a implementação do software de desenho com imagens em Raster, foram decisivos para o resultado final. Apesar dos inúmeros formatos pedidos, toda esta gestão só foi possível com uma estrutura bem delineada a nível de código em C, para que os ficheiros sejam gerados corretamente sem nenhuma perda de dados.
Em suma, pensamos ter cumprido com rigor aquilo a que este trabalho nos implicou ao nivel de conhecimento teórico e prático na disciplina de processamento de linguagens.]

## Bibliografia / Referências

- [GitHub Flex](https://github.com/westes/flex)
- [Bison Lecture](http://web.eecs.utk.edu/~bvanderz/teaching/cs461Sp11/notes/bison/)
