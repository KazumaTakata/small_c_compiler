#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../token/token.h"

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

struct Expression *ParseExpression(struct Parser *p)
{
    if (p->curtoken->Type == INT)
    {
        struct Expression *expression = (struct Expression *)malloc(sizeof(struct Expression));
        expression->token = p->curtoken->Type;
        expression->value = atoi(p->curtoken->Literal);
        return expression;
    }
    else
    {
        struct Expression *expression = (struct Expression *)malloc(sizeof(struct Expression));

        enum TokenType type = p->curtoken->Type;
        if (type == NEGATION || type == LNEGATION || type == BCOMP)
        {
            expression->Unary = type;
            NextToken(p);
            struct Expression *exp = ParseExpression(p);
            expression->right = exp;
        }

        return expression;
    }
}

struct Statement *ParseStatement(struct Parser *p)
{

    if (p->curtoken->Type == RETURN)
    {
        struct Statement *statement = (struct Statement *)malloc(sizeof(struct Statement));
        statement->token = p->curtoken->Type;
        NextToken(p);
        struct Expression *expression = ParseExpression(p);

        NextToken(p);
        if (p->curtoken->Type != SEMICOLON)
        {
            printf("error");
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
        printf("error");
    }

    NextToken(p);

    struct Statement *statement = ParseStatement(p);
    function->body = statement;

    if (p->curtoken->Type != RBRACE)
    {
        printf("error");
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