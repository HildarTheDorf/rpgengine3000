#ifndef RPG3000_RPG_H
#define RPG3000_RPG_H

#include "../common.h"

#ifdef _WIN32
bool REQUIRE_GETCHAR = true;
#elif defined __unix__ || (defined __APPLE__ && defined __MACH__)
bool REQUIRE_GETCHAR = false;
#endif

static const float PLAYER_VERSION = 0.0;

static struct datastruct * init(int, char *[]);
static void cleanup(int, struct datastruct *);
static void clearScreen(void);
static void spawnTerminal(int, char *[]);


#endif
