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
