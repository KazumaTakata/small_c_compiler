#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"
#include "../parser/parser.h"
#include "./generator.h"

int main(void)
{
    char input_string[] = "\
          int main(){ \
            return !0;\
        }";

    struct Lexer *l = createLexer(input_string);

    struct Parser *p = createParser(l);

    struct Program *program = ParseProgram(p);

    GenerateAsm(program);

    return 0;
}
