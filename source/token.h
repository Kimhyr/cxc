#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>
#include <stdint.h>

#include "diagnostic.h"

typedef enum : int8_t {
    $ = INT8_MIN,

    // Specials
    IDENTIFIER, // [a-zA-Z_][0-9a-zA-Z_]*
    STRING,     // "([^\\\n]|\\.|\\\n)*"
    UINTEGER,   // [0-9]+
    INTEGER,    // [+|\-]?uinteger
    FLOATING,   // integer\.?uinteger

    // Keywords
    FUNCTION, // function
    VALUE,    // value
    U8,       // u8
    U16,      // u16
    U32,      // u32
    U64,      // u64
    I8,       // i8
    I16,      // i16
    I32,      // i32
    I64,      // i64
    F16,      // f16
    F32,      // f32
    F64,      // f64

    END = 0,

    // Punctuators
    FORWARD_ARROW, // ->
    COLON             = ':',
    LEFT_PARENTHESIS  = '(',
    RIGHT_PARENTHESIS = ')',
    COMMA             = ',',
    SEMICOLON         = ';',
    PLUS              = '+',
    MINUS             = '-',
    DOT               = '.',


    _ = INT8_MAX,
} TokenType;

typedef union {
    const char* identifier;
    const char* string;
    size_t      uinteger;
    ptrdiff_t   integer;
    double      floating;
} TokenValue;

typedef struct {
    TokenValue value; 
    TokenType  type;
    Location   location;
} Token;

#endif
