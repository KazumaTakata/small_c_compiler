#include "./lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../token/token.h"

int main(void)
{
    char input_string[] = "\
          int main(){ \
            return 2;\
        }";

    struct Lexer *l = createLexer(input_string, 0);

    struct Token *token = nextToken(l);

    while (token->Type != EOFT)
    {
        free(token);
        token = nextToken(l);
    }

    return 0;
}
