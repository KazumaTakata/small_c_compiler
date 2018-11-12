#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"
#include "./parser.h"

int main(void)
{
    char input_string[] = "int main(){ \
            return -2 + 4 * 4;\
        }";

    struct Lexer *l = createLexer(input_string);

    struct Parser *p = createParser(l);

    struct Program *program = ParseProgram(p);

    return 0;
}
