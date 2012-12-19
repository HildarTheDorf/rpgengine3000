#include "files.h"


struct datastruct * readfile(FILE *gamefile)
{
    if (gamefile == NULL)
        return NULL;

    fseek(gamefile, 0, SEEK_SET);

    char magic[5] = "\0\0\0\0\0";
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);

    if (ret != 4 || strcmp(magic, MAGIC)) {
        puts("Game file invalid or corrupt\n");
        return NULL;
    }

    struct datastruct *Data = malloc(sizeof(struct datastruct));
    if (Data == NULL)
        return NULL;

    char *tmp = calloc(LARGEST_NAME, sizeof(char));
    if (tmp == NULL)
        return NULL;

    fscanf(gamefile, "%s = %f\n", tmp, &Data->BuiltWith);
    if (strcmp(tmp, "BuiltWith")) {
        puts("Game file invalid or corrupt\n");
        free(tmp);
        free(Data);
        return NULL;
    }

    if (Data->BuiltWith > FILES_VERSION) {
        printf("\nERROR: Game file too new, created with version %f\nPlease update the program.\n\n", Data->BuiltWith);
        free(Data);
        free(tmp);
        return NULL;
    }

    free(tmp);
    return Data;
}
