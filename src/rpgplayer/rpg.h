#ifndef RPG3000_P_RPG_H
#define RPG3000_P_RPG_H
#include "player.h"
#include "../common.h"

static void printDescription(const struct mapnode *);
static bool handleInput(const struct datastruct *, struct mapnode **, struct charstruct *);
static bool randomEvent(const struct datastruct *, struct mapnode **, struct charstruct *);
static void movePlayer(struct mapnode **, short);

#endif
