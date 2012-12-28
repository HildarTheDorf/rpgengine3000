#ifndef RPG3000_FILES_H
#define RPG3000_FILES_H

#include "../common.h"

#define LINE_SUCCESS 0
#define LINE_START 1
#define LINE_END 2
#define LINE_ERROR -1

static const short unsigned INVALID_ROOM = 13337;

static const float FILES_VERSION = 0.0;

static char * getDataLine(FILE *, char *, char *);
static int getAttributeLine(FILE *, char*);
static int getNodeLine(FILE *, struct mapnode *, struct datastruct *);

#define returnInvalid() {puts("Game file invalid or corrupt\n");return EXIT_INVALIDGAMEFILE;}
#define returnNew() {printf("\nERROR: Game file too new, created with version %f\nPlease update the program.\n\n", Data->BuiltWith);return EXIT_INVALIDGAMEFILE;}

#endif
