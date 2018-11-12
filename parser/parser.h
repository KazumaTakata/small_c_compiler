#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"

struct Parser;
struct Factory *ParseFactory(struct Parser *p);
struct Expression *ParseExpression(struct Parser *p);

struct Parser
{
    struct Lexer *l;
    struct Token *curtoken;
    struct Token *peektoken;
};

void NextToken(struct Parser *p)
{
    p->curtoken = p->peektoken;
    p->peektoken = nextToken(p->l);
};

struct Parser *createParser(struct Lexer *l)
{
    struct Parser *p = (struct Parser *)malloc(sizeof(struct Parser));
    p->l = l;
    NextToken(p);
    NextToken(p);
    return p;
}

struct Factor *ParseFactor(struct Parser *p)
{
    if (p->curtoken->Type == INT)
    {
        struct Factor *factor = (struct Factor *)malloc(sizeof(struct Factor));
        factor->type = VALUE;
        factor->token = p->curtoken->Type;
        factor->value = atoi(p->curtoken->Literal);
        return factor;
    }
    else if (p->curtoken->Type == LPAREN)
    {
        struct Factor *factor = (struct Factor *)malloc(sizeof(struct Factor));
        NextToken(p);
        struct Expression *expression = ParseExpression(p);
        factor->expression = expression;
    }
    else
    {
        struct Factor *factor = (struct Factor *)malloc(sizeof(struct Factor));

        enum TokenType type = p->curtoken->Type;
        if (type == NEGATION || type == LNEGATION || type == BCOMP)
        {
            factor->type = PREFIX;
            factor->Unary = type;
            NextToken(p);
            struct Factor *exp = ParseFactor(p);
            factor->right = exp;
        }

        return factor;
    }
}

struct Term *ParseTerm(struct Parser *p)
{
    struct Factor *leftterm = ParseFactor(p);
    struct Term *term = (struct Term *)malloc(sizeof(struct Term));
    term->Binary = NONE;
    term->Tleft = NULL;
    term->left = leftterm;

    NextToken(p);

    int firstflag = 0;

    while (p->curtoken->Type == MULTI || p->curtoken->Type == DIV)
    {
        if (firstflag == 0)
        {
            term->Binary = p->curtoken->Type;
            firstflag += 1;
            NextToken(p);
            struct Factor *rightterm = ParseFactor(p);
            term->right = rightterm;
        }
        else
        {
            struct Term *Newterm = (struct Term *)malloc(sizeof(struct Term));
            Newterm->Binary = p->curtoken->Type;
            NextToken(p);
            struct Factor *rightterm = ParseFactor(p);
            Newterm->Tleft = term;
            Newterm->right = rightterm;
            term = Newterm;
        }
    };

    return term;
};

struct Expression *ParseExpression(struct Parser *p)
{
    struct Term *leftterm = ParseTerm(p);
    struct Expression *expression = (struct Expression *)malloc(sizeof(struct Expression));
    expression->Binary = NONE;
    expression->Eleft = NULL;
    expression->left = leftterm;

    int firstflag = 0;
    expression->Binary = NONE;

    while (p->curtoken->Type == ADD || p->curtoken->Type == NEGATION)
    {
        if (firstflag == 0)
        {
            expression->Binary = p->curtoken->Type;
            firstflag += 1;
            NextToken(p);
            struct Term *rightterm = ParseTerm(p);
            expression->right = rightterm;
        }
        else
        {
            struct Expression *Newexpression = (struct Expression *)malloc(sizeof(struct Expression));
            Newexpression->Binary = p->curtoken->Type;
            NextToken(p);
            struct Term *rightterm = ParseTerm(p);
            Newexpression->Eleft = expression;
            Newexpression->right = rightterm;
            expression = Newexpression;
        }
    };

    return expression;
};

struct Statement *ParseStatement(struct Parser *p)
{

    if (p->curtoken->Type == RETURN)
    {
        struct Statement *statement = (struct Statement *)malloc(sizeof(struct Statement));
        statement->token = p->curtoken->Type;
        NextToken(p);
        struct Expression *expression = ParseExpression(p);

        if (p->curtoken->Type != SEMICOLON)
        {
            printf("not semicolon get %d\n", p->curtoken->Type);
        }

        NextToken(p);

        statement->expression = expression;
        return statement;
    }
}

struct Function *ParseFunction(struct Parser *p)
{
    struct Function *function = (struct Function *)malloc(sizeof(struct Function));
    function->next = NULL;
    if (p->curtoken->Type != INT)
    {
        printf("not int get %d\n", p->curtoken->Type);
    }
    function->returnType = p->curtoken->Type;
    NextToken(p);
    if (p->curtoken->Type != IDENT)
    {
        printf("not ident %d\n", p->curtoken->Type);
    }
    function->ident = p->curtoken->Literal;
    NextToken(p);
    NextToken(p);
    NextToken(p);
    if (p->curtoken->Type != LBRACE)
    {
        printf("not lbrace get %d\n", p->curtoken->Type);
    }

    NextToken(p);

    struct Statement *statement = ParseStatement(p);
    function->body = statement;

    if (p->curtoken->Type != RBRACE)
    {
        printf("not rbrace get %d\n", p->curtoken->Type);
    }

    NextToken(p);

    return function;
}

struct Program *ParseProgram(struct Parser *p)
{
    struct Program *program = (struct Program *)malloc(sizeof(struct Program));
    struct Function *function = ParseFunction(p);
    program->function = function;
    while (p->curtoken->Type != EOFT)
    {
        struct Function *function = ParseFunction(p);
        program->function->next = function;
    }

    return program;
};

#endif