#ifndef AST_H
#define AST_H

#include "../token/token.h"

enum VariableType
{
    V_INT,
    V_BOOL,
    V_VOID
};

struct Expression
{
    enum TokenType token;
    int value;
};

struct Statement
{
    struct Statement *next;
    enum TokenType token;
    struct Expression *expression;
};

struct Function
{
    char *ident;
    enum TokenType returnType;
    struct Function *next;
    struct Statement *body;
};

struct Program
{
    struct Function *function;
};

#endif