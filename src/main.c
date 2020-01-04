/**
 * @file main.c
 * @author
 *  - José Moura <a13742|at|alunos.ipca.pt>
 *  - Óscar Silva <a14383|at|alunos.ipca.pt>
 *  - Daniel Filipe <a17442|at|alunos.ipca.pt>
 * @date 01 Jan 2020
 * @brief
 * Trata-se de uma aplicação de maniputação de imagens PNM do tipo P6
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/commands.h"
#include "../include/grammar.tab.h"

int main() {
    srand(time(0));

    yyparse();
    return 0;
}
