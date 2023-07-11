#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct {
    char*  pointer;
    size_t size;
    size_t capacity;
} Buffer;

void buffer(Buffer*, size_t capacity);
void push_buffer(Buffer*, char in);
void destroy_buffer(Buffer*);
void expand_buffer(Buffer*);
void shrink_buffer();

#endif
	