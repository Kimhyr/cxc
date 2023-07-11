#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#include "token.h"

typedef struct {
    const char* iterator;
    Position    position;
    int         fd;
} Lexer;

void lexer(Lexer*, const char *file_path);
bool lex  (Lexer*, Token* token);

#endif
