#ifndef AST_H
#define AST_H

#include "../token/token.h"

enum VariableType
{
    V_INT,
    V_BOOL,
    V_VOID
};

enum NodeType
{
    PREFIX,
    INFIX,
    VALUE,
};

struct Factory
{
    enum TokenType Unary;
    enum TokenType token;
    enum NodeType type;
    int value;
    struct Factory *right;
    struct Expression *expression;
};

struct Term
{
    enum TokenType Binary;
    struct Factory *left;
    struct Factory *right;
    struct Term *Tleft;
};

struct Expression
{
    enum TokenType Binary;
    struct Term *left;
    struct Term *right;
    struct Expression *Eleft;
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