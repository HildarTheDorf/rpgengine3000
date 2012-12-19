#include "rpg.h"
#include "../common/files.h"

static FILE * init(char*);
static int cleanup(struct datastruct *);

int main(int argc, char *argv[])
{
    system("clear");
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    if (argc == 1) {
        printf("Error: insufficent arguments\nUsage: %s gamefile\n\n", argv[0]);
        return EXIT_ARGUMENTS;
    }

    FILE *gamefile = init(argv[1]);

    if (gamefile == NULL)
        return EXIT_NOGAMEFILE;

    struct datastruct *Data = readfile(gamefile);
    fclose(gamefile);

    if (Data == NULL) {
        return EXIT_INVALIDGAMEFILE;
    }

    printf("Load succesful!\nGame: %s\nCreator: %s\nVersion: %.1f\nBuilt with rpgcreator version: %.1f\n", "NYI", "NYI", 0.0, Data->BuiltWith);

    return cleanup(Data);    
}

static FILE * init(char* filename)
{
    printf("Loading game file %s...\n", filename);

    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        return NULL;
    }

    char magic[5] = "\0\0\0\0\0";
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);

    if (ret != 4 || strcmp(magic, MAGIC)) {
        printf("Game file %s invalid or corrupt\n", filename);
        fclose(gamefile);
        return NULL;
    }

    return gamefile;
}

static int cleanup(struct datastruct *Data)
{
    free(Data);
    return EXIT_SUCCESS;
}
