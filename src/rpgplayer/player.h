#ifndef RPG3000_P_PLAYER_H
#define RPG3000_P_PLAYER_H

#include "../common.h"

static const float PLAYER_VERSION = 0.0;

int mainLoop(const struct datastruct *, struct mapnode *, struct charstruct *);
void clearScreen(void);
bool requireGetchar(bool);

// Flushes stdin to the next line or EOF, whichever comes first.
// Input takes the character you just getchar()'d.
static inline int flushchar(char input)
{
    if (input == '\n' || input == EOF)
        return 1;

    while (input != '\n' && input != EOF)
        input = getchar();

    return 0;
}

// 'Safe' malloc(). Checks for NULL and quits if program is out of memory.
static inline void * smalloc(size_t size)
{
    void *pointer = malloc(size);
    // If malloc fails there's presumably no heap left.
    // Therefore there is a major (possibly system-wide) error!
    // Panic.
    if (pointer == NULL) { 
        fprintf(stderr, "\n***CRITICAL ERROR: Could not allocate %zu bytes of memory!***\n", size);
        if (requireGetchar(0))
            getchar();
        exit(EXIT_OOM);
    }
    return pointer;
}

#endif
