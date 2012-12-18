#include "rpg.h"

static FILE * init(char*);

int main(int argc, char *argv[])
{
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", VERSION);

    if (argc != 2) {
        printf("Error: insufficent arguments\nUsage: %s gamefile\n", argv[0]);
        return EXIT_ARGUMENTS;
    }

    char *filename = argv[1];

    FILE *gamefile = init(argv[1]);

    if (gamefile == NULL)
        return EXIT_NOGAMEFILE;

    printf("Game file %s loaded. This game was built with rpgcreator3000 version %.1f\n\n", filename, 0.0);

    fclose(gamefile);    
    return EXIT_SUCCESS;
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
    ret = fread(magic, sizeof(char), 3, gamefile);

    if (ret != 3 || *magic != *MAGIC) {
        printf("Game file %s invalid or corrupt\n", filename);
        return NULL;
    }

    return gamefile;
}
