#ifndef RPG3000_RPG_H
#define RPG3000_RPG_H

#include "../common.h"

static const float PLAYER_VERSION = 0.0;

static struct datastruct * init(char*);
static void cleanup(int, struct datastruct *);
static void clearScreen(void);


#endif
