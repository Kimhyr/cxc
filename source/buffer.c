#include <stdlib.h>

#include "buffer.h"

void buffer(Buffer* b)
{
    b->size = 0;
    b->capacity = 4;
    b->pointer = malloc(4);
}

void push_buffer(Buffer* b, char i)
{
    if (b->capacity < b->size + 1) {
        expand_buffer(b);
    }

    *++b->pointer = i;
}

void expand_buffer(Buffer* b)
{
    b->capacity = b->capacity * (b->capacity << 1);
    b->pointer = realloc(b->pointer, b->capacity);
}


void shrink_buffer(Buffer* b)
{
    b->pointer = realloc(b->pointer, b->size);
    b->capacity = b->size;
}
