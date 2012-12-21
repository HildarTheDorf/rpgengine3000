#ifndef RPG3000_FILES_H
#define RPG3000_FILES_H

#include "../common.h"

#define LINE_SUCCESS 0
#define LINE_START 1
#define LINE_END 2
#define LINE_ERROR -1

static const float FILES_VERSION = 0.0;

static char * getDataLine(FILE *, char *, char *);
static int getAttributeLine(FILE *, char*);

#endif
