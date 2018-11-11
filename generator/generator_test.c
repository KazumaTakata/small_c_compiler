#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"
#include "../parser/parser.h"

int main(void)
{
    char input_string[] = "\
          int main(){ \
            return 2;\
        }";

    struct Lexer *l = createLexer(input_string, 0);

    struct Parser *p = createParser(l);

    struct Program *program = ParseProgram(p);

    return 0;
}
