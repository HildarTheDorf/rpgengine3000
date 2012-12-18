#ifndef FILES_H
#define FILES_H

#include <stdio.h>

static const float FILES_VERSION = 0.0;

struct datastruct {
    float BuiltWith;
    float Version;
    char *Name;
    char *Title;
    char *Description;    
};

struct datastruct * readfile(FILE *);

#endif
