#ifndef RPG3000_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define EXIT_ARGUMENTS 1
#define EXIT_NOGAMEFILE 2
#define EXIT_INVALIDGAMEFILE 3
#define EXIT_NEVEROCCUR 254
#define EXIT_OOM 255

#define LARGEST_NAME 32
#define LARGEST_DATA 256
static const unsigned short LARGEST_LINE = LARGEST_NAME + LARGEST_DATA + 3; //name + '=' + data + '\n' + '\0'

struct datastruct {
    float BuiltWith;
    float Version;
    char Creator[LARGEST_DATA];
    char Title[LARGEST_DATA];
    char Description[LARGEST_DATA];    
};

struct datastruct * readfile(FILE *);
static const char MAGIC[] = "GEW\n";

#endif
