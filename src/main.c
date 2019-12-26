#include "../src/commands.h"
#include "../include/grammar.tab.h"

int main() {
    yyparse();
    return 0;
}
