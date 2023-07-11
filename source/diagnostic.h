#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stddef.h>
#include <stdio.h>

#include "array.h"

typedef struct {
    size_t row;
    size_t column;
} Position;

typedef struct {
    Position start;
    Position end;
} Span;

typedef struct Path {
    struct Path* prior;
    struct Path* next;
    Buffer       buffer;
} Path;

typedef struct {
    const char *path; 
    Span        span;
} Location;

typedef enum {
    MORE_THAN_ONE_PERIOD_IN_FLOATING_TOKEN,
} Error;

void print_error(Error, Location* restrict);

#endif
