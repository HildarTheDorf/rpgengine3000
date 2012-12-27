#ifndef RPG3000_COMMON_H
#define RPG3000_COMMON_H

#ifdef _WIN32
#   include <windows.h>
#elif defined __unix__ || (defined __APPLE__ && defined __MACH__)
#   define _XOPEN_SOURCE 700
#   include <unistd.h>
#else
#   error "Please compile for windows or a POSIX compliant system. If you believe this is in error, define _WIN32 or __unix__ as appropriate."
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

enum {
    EXIT_ARGUMENTS = 1,
    EXIT_NOGAMEFILE,
    EXIT_INVALIDGAMEFILE,
    EXIT_NOTINTERACTIVE,
    EXIT_INVALIDPOINTER,

    EXIT_OOM = 255,
};

#define LARGEST_ROOM_NAME 32
#define LARGEST_ROOM_DESC 256
#define LARGEST_EXIT_NAME 32

static const unsigned short NORTH_VALID  = 0x01;
static const unsigned short SOUTH_VALID  = 0x02;
static const unsigned short EAST_VALID   = 0x04;
static const unsigned short WEST_VALID   = 0x08;
static const unsigned short UP_VALID     = 0x10;
static const unsigned short DOWN_VALID   = 0x20;
static const unsigned short EXTRA0_VALID = 0x40;
static const unsigned short EXTRA1_VALID = 0x80;

static const unsigned short ROOM_FLAGS_DARK = 0x01;

enum {
    EXIT_NORTH = 0,
    EXIT_SOUTH,
    EXIT_EAST,
    EXIT_WEST,
    EXIT_UP,
    EXIT_DOWN,
    EXIT_EXTRA0,
    EXIT_EXTRA1,
};
// Change EXIT_MAX and an _VALID flag if you add extra exits to the above enum.
#define EXIT_MAX EXIT_EXTRA1 + 1

// This struct, and the latter datastruct one, have their contents ordered oddly to save space due to alignment.
// This is an optimization specific to Linux x64, but should be reasonably valid on most none-estoric systems.
struct mapnode {
    char Desc[LARGEST_ROOM_DESC];
    char ExitName[EXIT_MAX - 6][LARGEST_EXIT_NAME];
    struct mapnode *Exit[EXIT_MAX];
    char Name[LARGEST_ROOM_NAME];
    unsigned short ID;
    unsigned short ValidExits;
    unsigned short Flags;
    char ExitLetter[EXIT_MAX - 6];
};

#define LARGEST_NAME 32
#define LARGEST_DATA 256
static const unsigned short LARGEST_LINE = LARGEST_NAME + LARGEST_DATA + 3; //name + '=' + data + '\n' + '\0'

#define LARGEST_ATTRIB_NUM 15
#define MAX_MAP_SIZE 4096

struct datastruct {
    struct mapnode Map[MAX_MAP_SIZE];
    char Attributes[LARGEST_ATTRIB_NUM][LARGEST_DATA];
    char Creator[LARGEST_DATA];
    char Title[LARGEST_DATA];
    char Description[LARGEST_DATA];
    float BuiltWith;
    float Version;
    short unsigned NumAttributes;
};

static const char MAGIC[] = "GEW\n";

int readfile(FILE * gamefile, struct datastruct *Data);

#endif
