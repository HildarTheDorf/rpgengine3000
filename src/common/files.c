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

    char line[LARGEST_LINE];
    char *value;

    // Get a line from the file, find the first = sign and convert it to a NULL char. Value is a pointer to the first char after the =.
    fgets(line, sizeof(line), gamefile);
    value = (strchr(line, '='));
    *(value++) = '\0';

    if (strcmp(line, "BuiltWith")) {
        puts("Game file invalid or corrupt\n");
        free(Data);
        return NULL;
    }

    Data->BuiltWith = atof(value);

    if (Data->BuiltWith > FILES_VERSION) {
        printf("\nERROR: Game file too new, created with version %f\nPlease update the program.\n\n", Data->BuiltWith);
        free(Data);
        return NULL;
    }

    return Data;
}
