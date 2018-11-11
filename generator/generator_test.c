#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"
#include "../parser/parser.h"

void GenerateExpression(struct Expression *expression)
{
    if (expression->token == INT)
    {
        int value = expression->value;
    }
};

void GenerateStatement(struct Statement *statement, FILE *fp)
{
    if (statement->token == RETURN)
    {
        struct Expression *expression = statement->expression;
        fprintf(fp, "\t mov rax, %d\n", expression->value);
        fprintf(fp, "\t leave\n");
        fprintf(fp, "\t ret\n");
    }
}

void GenerateFunc(struct Function *func, FILE *fp)
{
    char *ident = func->ident;
    struct Statement *statement = func->body;

    fprintf(fp, "_%s:\n", ident);
    fprintf(fp, "\t push rbp\n");
    fprintf(fp, "\t mov rbp, rsp\n");
    fprintf(fp, "\t sub rsp, 16\n");

    GenerateStatement(statement, fp);
};

void GenerateAsm(struct Program *p, FILE *fp)
{
    struct Function *func = p->function;
    while (1)
    {
        GenerateFunc(func, fp);
        if (func->next == NULL)
        {
            break;
        }
        func = func->next;
    }
}

int main(void)
{
    char input_string[] = "\
          int main(){ \
            return 2;\
        }";

    struct Lexer *l = createLexer(input_string);

    struct Parser *p = createParser(l);

    struct Program *program = ParseProgram(p);

    FILE *fp = fopen("./main.asm", "w");

    fprintf(fp, "\t global _main\n");
    fprintf(fp, "\t segment .text\n");

    GenerateAsm(program, fp);

    fclose(fp);

    return 0;
}
