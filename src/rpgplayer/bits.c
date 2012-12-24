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

static inline void *smalloc(size_t size)
{
    void *pointer = malloc(size);
    // If malloc fails there's presumably no heap left.
    // Therefore there is a major (possibly system-wide) error!
    // Panic.
    if (pointer == NULL) { 
        puts("\n***CRITICAL ERROR: Could not allocate memory!***\n");
        if (REQUIRE_GETCHAR)
            getchar();
        exit(EXIT_OOM);
    }
    return pointer;
}

#endif
