#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const float FILES_VERSION = 0.0;
#define LARGEST_NAME 32
#define LARGEST_DATA 256
static const unsigned short LARGEST_LINE = LARGEST_NAME + LARGEST_DATA + 3; //name + '=' + data + '\n' + '\0'

struct datastruct {
    float BuiltWith;
    float Version;
    char *Name;
    char *Title;
    char *Description;    
};

struct datastruct * readfile(FILE *);

static const char MAGIC[] = "GEW\n";

#endif
