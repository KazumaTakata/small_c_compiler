#ifndef GENERATOR_H
#define GENERATOR_H

#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"
#include "../parser/parser.h"

void GenerateFactor(struct Factor *factor, FILE *fp)
{
    if (factor->type == VALUE)
    {
        int value = factor->value;
        fprintf(fp, "\t mov rax, %d\n", factor->value);
    }
    else if (factor->type == PREFIX)
    {
        GenerateFactor(factor->right, fp);

        enum TokenType unaryType = factor->Unary;

        switch (unaryType)
        {
        case NEGATION:
            fprintf(fp, "\t neg rax\n");
            break;
        case BCOMP:
            fprintf(fp, "\t not rax\n");
            break;
        case LNEGATION:
            fprintf(fp, "\t cmp rax, 0\n");
            fprintf(fp, "\t mov rax, 0\n");
            fprintf(fp, "\t sete al\n");
            break;
        }
    }
};

void GenerateTerm(struct Term *term, FILE *fp)
{
    if (term->Tleft == NULL)
    {
        struct Factor *leftfactor = term->left;
        GenerateFactor(leftfactor, fp);
    }
    else
    {
        struct Term *leftterm = term->Tleft;
        GenerateTerm(leftterm, fp);
    }
    if (term->Binary != NONE)
    {
        fprintf(fp, "\t push rax\n");
        struct Factor *rightfactor = term->right;
        GenerateFactor(rightfactor, fp);

        fprintf(fp, "\t pop rcx\n");
        fprintf(fp, "\t imul rax, rcx\n");
    };
}

void GenerateExpression(struct Expression *expression, FILE *fp)
{
    if (expression->Eleft == NULL)
    {
        struct Term *leftfactor = expression->left;
        GenerateTerm(leftfactor, fp);
    }
    else
    {
        struct Expression *leftterm = expression->Eleft;
        GenerateExpression(leftterm, fp);
    };
    if (expression->Binary != NONE)
    {
        struct Term *rightfactor = expression->right;
        fprintf(fp, "\t push rax\n");
        GenerateTerm(rightfactor, fp);

        fprintf(fp, "\t pop rcx\n");
        fprintf(fp, "\t add rax, rcx\n");
    }
}

void GenerateStatement(struct Statement *statement, FILE *fp)
{
    if (statement->token == RETURN)
    {
        struct Expression *expression = statement->expression;
        GenerateExpression(expression, fp);
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

void GenerateAsm(struct Program *p)
{
    FILE *fp = fopen("./main.asm", "w");

    fprintf(fp, "\t global _main\n");
    fprintf(fp, "\t segment .text\n");
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

    fclose(fp);
};

#endif