#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "lexer.h"
#include "token.h"
#include "utilities.h"

static        void skip_whitespace(Lexer*);
static        bool is_whitespace(char);
static        void next(Lexer*);
static inline char peek(Lexer*, char offset);
static inline char current(Lexer*);
static        void number(Lexer*, Token*);
static        void uinteger(Lexer*, Token*);
static inline void token(Token*, Position* restrict);

void lexer(Lexer* l, const char* restrict fp)
{
    l->fd = fmmap(fp, (void*)l->iterator);
    l->position.row = 1;
    l->position.column = 1;
}

bool lex(Lexer* l, Token* t)
{
    skip_whitespace(l);
    char c = current(l);
    token(t, &l->position);
    switch (c) {
    case PLUS:
    case MINUS:
        if (isdigit(peek(l, 1))) {
            t->type = INTEGER;
            number(l, t);
            break;
        }
    case COLON:
    case LEFT_PARENTHESIS:
    case RIGHT_PARENTHESIS:
    case COMMA:
    case SEMICOLON:
    case EOF:
        t->type = c;
        break;
    default:
        if (isdigit(c)) {
            
        }
        break;
    }
    return false;
}

void token(Token* t, Position* position)
{
    t->location.span.start = *position;
    t->location.span.end.row = 0;
    t->location.span.end.column = 0;
}

void number(Lexer* l, Token* t)
{
    Buffer b;
    buffer(&b);

    char c = current(l);
    do {
        push_buffer(&b, c);
        next(l);
        c = current(l);
        if (c == '\0') {
            break;
        }

        if (c == DOT) {
            if (t->type == FLOATING) {
                t->location.span.end = l->position;
                print_error(MORE_THAN_ONE_PERIOD_IN_FLOATING_TOKEN, &t->location);
                exit(1);
            }
            t->type = FLOATING;
            continue;
        }
    } while (isdigit(c));
    shrink_buffer(&b);
}

void skip_whitespace(Lexer* l)
{
    char c = current(l);
    while (is_whitespace(c)) {
        next(l);
        c = current(l);
    }
}

bool is_whitespace(char c)
{
    return c == ' ' || (c >= '\t' && c <= '\r');
}

void next(Lexer* l)
{
    char c = current(l);
    if (c == '\n') {
        ++l->position.row;
        l->position.column = 0;
    }
    ++l->position.column;
}

char current(Lexer* l)
{
    return *l->iterator;
}

char peek(Lexer* l, char o)
{
    return l->iterator[o];
}
