#ifndef RPG3000_P_MAIN_H
#define RPG3000_P_MAIN_H

#include "../common.h"
#include "player.h"

#ifndef __GNUC__
#   define __attribute__(x) /* Defined as nothing (i.e. removed) if unsupported. */
#endif

#ifdef _WIN32
bool REQUIRE_GETCHAR = true;
#else
bool REQUIRE_GETCHAR = false;
#endif

static void initGlobal(int, char *[]); 
static void initData(char *, struct datastruct *);
static void initMap(struct datastruct *, struct mapnode **);
static int cleanup(int, struct datastruct *);
#if defined __unix__ || (defined __APPLE__ && __MACH__)
static void spawnTerminal(int, char *[]) __attribute__((noreturn));
#endif

#endif
