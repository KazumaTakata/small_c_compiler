#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../token/token.h"

struct Lexer
{
    char *input_string;
    int position;
    int readPosition;
    char ch;
};

void readChar(struct Lexer *l)
{
    if (l->readPosition >= strlen(l->input_string))
    {
        l->ch = 0;
    }
    else
    {
        l->ch = l->input_string[l->readPosition];
    }
    l->position = l->readPosition;
    l->readPosition += 1;
}

struct Lexer *createLexer(char *input_string)
{
    struct Lexer *l = (struct Lexer *)malloc(sizeof(struct Lexer));
    l->position = 0;
    l->readPosition = 0;
    l->input_string = strdup(input_string);
    readChar(l);
    return l;
}

void skipWhitespace(struct Lexer *l)
{
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r')
    {
        readChar(l);
    }
}

int isLetter(char ch)
{
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

int isNumber(char ch)
{
    return ('0' <= ch && ch <= '9') || ch == '.';
}

void slice_str(const char *str, char *buffer, size_t start, size_t end)
{
    size_t j = 0;
    for (size_t i = start; i <= end; ++i)
    {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
}

void readIdentifier(struct Lexer *l, char buffer[])
{
    int po = l->position;

    while (isLetter(l->ch))
    {
        readChar(l);
    }
    slice_str(l->input_string, buffer, po, l->position - 1);
}

void readNumber(struct Lexer *l, char buffer[])
{
    int po = l->position;

    while (isNumber(l->ch))
    {
        readChar(l);
    }
    slice_str(l->input_string, buffer, po, l->position - 1);
}

struct Token *nextToken(struct Lexer *l)
{
    struct Token *token;
    skipWhitespace(l);
    switch (l->ch)
    {
    case '{':
        token = createToken(LBRACE, &l->ch);
        readChar(l);
        break;
    case '}':
        token = createToken(RBRACE, &l->ch);
        readChar(l);
        break;
    case '(':
        token = createToken(LPAREN, &l->ch);
        readChar(l);
        break;
    case ')':
        token = createToken(RPAREN, &l->ch);
        readChar(l);
        break;
    case ';':
        token = createToken(SEMICOLON, &l->ch);
        readChar(l);
        break;
    case '-':
        token = createToken(NEGATION, &l->ch);
        readChar(l);
        break;
    case '~':
        token = createToken(BCOMP, &l->ch);
        readChar(l);
        break;
    case '=':
        if (l->input_string[l->readPosition] == '=')
        {
            char equal[] = "==";
            token = createToken(EQUAL, equal);
            readChar(l);
            readChar(l);
        }
        else
        {
            token = createToken(ASSIGN, &l->ch);
            readChar(l);
        }
        break;
    case '!':
        if (l->input_string[l->readPosition] == '=')
        {
            char notequal[] = "!=";
            token = createToken(NOTEQUAL, notequal);
            readChar(l);
            readChar(l);
        }
        else
        {
            token = createToken(LNEGATION, &l->ch);
            readChar(l);
        }
        break;
    case '+':
        token = createToken(ADD, &l->ch);
        readChar(l);
        break;
    case '*':
        token = createToken(MULTI, &l->ch);
        readChar(l);
        break;
    case '/':
        token = createToken(DIV, &l->ch);
        readChar(l);
        break;
    case 0:
        token = createToken(EOFT, &l->ch);
        readChar(l);
        break;
    default:
        if (isLetter(l->ch))
        {
            const size_t len = strlen(l->input_string);
            char ident[len + 1];
            readIdentifier(l, ident);
            enum TokenType type = LookupIdent(ident);
            token = createToken(type, ident);
        }
        else if (isNumber(l->ch))
        {
            const size_t len = strlen(l->input_string);
            char number[len + 1];
            readNumber(l, number);
            enum TokenType type = INT;
            token = createToken(type, number);
        }
        else
        {
            token = createToken(ILLEGAL, &l->ch);
        }

        break;
    }

    return token;
};

#endif