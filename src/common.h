#ifndef RPG3000_COMMON_H
#   define RPG3000_COMMON_H
#   include <stdlib.h>
#   include <stdio.h>
#   include <string.h>
#   include <stdbool.h>
#   ifdef _WIN32
#       include <windows.h>
#   elif defined __unix__ || (defined __APPLE__ && defined __MACH__)
#       define _XOPEN_SOURCE 700
#       include <unistd.h>
#   else
#       error "Please compile for windows or a POSIX compliant system. If you believe this is in error, define _WIN32 or __unix__ as appropriate."
#   endif

enum {
    EXIT_ARGUMENTS = 1,
    EXIT_NOGAMEFILE,
    EXIT_INVALIDGAMEFILE,
    EXIT_NOTINTERACTIVE,
    EXIT_INVALIDPOINTER,

    EXIT_OOM = 255,
};

#define LARGEST_NAME 32
#define LARGEST_DATA 256
static const unsigned short LARGEST_LINE = LARGEST_NAME + LARGEST_DATA + 3; //name + '=' + data + '\n' + '\0'

#define LARGEST_ATTRIB_NUM 6

struct datastruct {
    float BuiltWith;
    float Version;
    char Creator[LARGEST_DATA];
    char Title[LARGEST_DATA];
    char Description[LARGEST_DATA];
    short unsigned NumAttributes;
    char Attributes[LARGEST_ATTRIB_NUM][LARGEST_DATA];
};

extern bool REQUIRE_GETCHAR;

static const char MAGIC[] = "GEW\n";

int readfile(FILE * gamefile, struct datastruct *Data);

#endif
