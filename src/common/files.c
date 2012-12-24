#include "files.h"

struct datastruct * readfile(FILE *gamefile)
{
    // We should really have already checked for this.
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
    // Let's create the main data structure
    struct datastruct *Data = malloc(sizeof(struct datastruct));
    // If malloc fails we can't just return NULL since it will be assumed the file is invalid.
    // And if there's no heap left, there's a major system-wide error!
    // Panic.
    if (Data == NULL) { 
        puts("\n***CRITICAL ERROR: Could not allocate memory!***\n");
        if (REQUIRE_GETCHAR)
            getchar();
        exit(EXIT_OOM);
    }

    char name[LARGEST_LINE];
    name[0] = '\0';
    char *value = NULL;
    // Check the BuiltWith value and ensure we aren't opening a version with unknown format.
    // This means BuiltWith MUST be the 2nd line, directly after the magic number
    value = getDataLine(gamefile, name, value);

    if (strcmp(name, "BuiltWith")) {
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

    // Keep going over the lines and placing the data into the appropriate places.
    while (value != NULL) {
        value = getDataLine(gamefile, name, value);

        if (!strcmp(name, "Creator"))
            strncpy(Data->Creator, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Title"))
            strncpy(Data->Title, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Description"))
            strncpy(Data->Description, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Version"))
            Data->Version = atof(value);
        if (!strcmp(name, "Attributes")) {
            if (sscanf(value, "%hu", &Data->NumAttributes) != 1) {
                puts("Game file invalid or corrupt\n");
                free(Data);
                return NULL;
            }
            for (short i = 0; i < Data->NumAttributes; ++i) {
                switch (getAttributeLine(gamefile, name)) {
                case (LINE_ERROR) :
                    puts("Game file invalid or corrupt\n");
                    free(Data);
                    return NULL;
                case (LINE_START) :
                    --i;
                    break;
                case (LINE_END) :
                    i = LARGEST_ATTRIB_NUM + 1;
                    break;
                case (LINE_SUCCESS) :
                    strcpy(Data->Attributes[i], name);
                }
            }
        }
    }

    return Data;
}

static char * getDataLine(FILE *gamefile, char *line, char *value)
{
    // Get a line from the file, find the first = sign and convert it to a NULL char. Return is a pointer to the first char after the =.
    if (fgets(line, LARGEST_LINE * sizeof(char), gamefile) == NULL)
        return NULL;
    value = NULL;
    value = (strchr(line, '='));
    if (value == NULL)
        return NULL;
    *(value++) = '\0';
    return value;
}

static int getAttributeLine(FILE *gamefile, char *line)
{
    // Get a line and remove the final \n
    if (fgets(line, LARGEST_LINE * sizeof(char), gamefile) == NULL)
        return LINE_ERROR;
    if (!strcmp(line, "{\n"))
        return LINE_START;
    if (!strcmp(line, "}\n"))
        return LINE_END;
    
    char *end = (strchr(line, '\n'));
    *end = '\0';
    return LINE_SUCCESS;
}
