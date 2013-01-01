#ifndef RPG3000_P_MAIN_H
#define RPG3000_P_MAIN_H

#include "../common.h"
#include "player.h"

#ifndef __GNUC__
#   define __attribute__(x) /* Defined as nothing (i.e. removed) if unsupported. */
#endif

static void initGlobal(int, char *[]); 
static void initData(char *, struct datastruct *);
static void initMap(struct datastruct *, struct mapnode **);
static void initPlayer(const struct datastruct *Data, struct charstruct *Player);
static int cleanup(int, struct datastruct *, struct charstruct *);
#if defined __unix__ || (defined __APPLE__ && __MACH__)
static void spawnTerminal(int, char *[]) __attribute__((noreturn));
#endif

#endif
