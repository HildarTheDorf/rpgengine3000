#include "files.h"

struct datastruct * readfile(FILE *gamefile)
{
    if (gamefile == NULL)
        return NULL;

    fseek(gamefile, 0, SEEK_SET);

    // This should really be checked anyway on opening the file. But we want to get back to a sane locaton in the file (4, SEEK_SET) anyway. Can't hurt to double check ^^
    char magic[5] = "\0\0\0\0\0";
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);

    if (ret != 4 || strcmp(magic, MAGIC)) {
        puts("Game file invalid or corrupt\n");
        return NULL;
    }
    // Gogo main data structure
    struct datastruct *Data = malloc(sizeof(struct datastruct));
    if (Data == NULL) //If this happens, PANIC AND GET OUT OF HERE!
        exit(EXIT_OOM);

    char line[LARGEST_LINE];
    line[0] = '\0';
    char *value = NULL;
    // Check the BuiltWith value and ensure we aren't opening a version with unknown format.
    // This means BuiltWith MUST be the 2nd line, directly after the magic number
    // There is no restriction on what order other lines come.
    value = getDataLine(gamefile, line, value);

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
    // Keep going over the lines and pushing them into the appropriate places.
    while (value != NULL) {
        value = getDataLine(gamefile, line, value);

        if (!strcmp(line, "Creator"))
            strncpy(Data->Creator, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(line, "Title"))
            strncpy(Data->Title, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(line, "Description"))
            strncpy(Data->Description, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(line, "Version"))
            Data->Version = atof(value);
    }

    return Data;
}

static char * getDataLine(FILE *gamefile, char *line, char *value)
{
    // Get a line from the file, find the first = sign and convert it to a NULL char. Return is a pointer to the first char after the =.
    value = NULL;
    fgets(line, LARGEST_LINE * sizeof(char), gamefile);
    value = (strchr(line, '='));
    if (value == NULL)
        return NULL;
    *(value++) = '\0';
    return value;
}
