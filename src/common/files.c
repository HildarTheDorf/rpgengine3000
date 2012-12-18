#include "files.h"


struct datastruct * readfile(FILE *gamefile)
{
    if (gamefile == NULL)
        return NULL;

    fseek(gamefile, 0, SEEK_SET);
    
    printf("DEBUG: files.c:readfile(). Not yet implemented\n");
    return NULL;
}
