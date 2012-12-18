#ifndef BITS_H
#define BITS_H
#include "rpg.h"

static inline int flushchar(char input)
{
    if (input == '\n' || input == EOF)
        return 1;

    while (input != '\n' && input != EOF)
        input = getchar();

    return 0;
}

static inline void *smalloc(long unsigned size)
{
    void *pointer = malloc(size);
    if (pointer == NULL)
        exit(EXIT_OOM);
    return pointer;
}

#endif
