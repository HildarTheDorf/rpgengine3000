#ifndef FILES_H
#define FILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const float FILES_VERSION = 0.0;
static const unsigned short LARGEST_NAME = 32;
static const unsigned short LARGEST_DATA = 256;

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
