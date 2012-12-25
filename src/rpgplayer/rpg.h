#ifndef RPG3000_RPG_H
#define RPG3000_RPG_H

#include "../common.h"

#ifdef _WIN32
bool REQUIRE_GETCHAR = true;
#else
bool REQUIRE_GETCHAR = false;
#endif

static const float PLAYER_VERSION = 0.0;

static void initGlobal(int, char *[]); 
static void initData(char *, struct datastruct *);
static int cleanup(int, struct datastruct *);
static void clearScreen(void);
static void spawnTerminal(int, char *[]) __attribute__((noreturn));


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
        printf("\n***CRITICAL ERROR: Could not allocate %lu bytes of memory!***\n", (long unsigned)size);
        if (REQUIRE_GETCHAR)
            getchar();
        exit(EXIT_OOM);
    }
    return pointer;
}


#endif
