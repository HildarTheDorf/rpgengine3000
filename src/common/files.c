#include "files.h"

int readfile(FILE *gamefile, struct datastruct *Data)
{
    // We should really have already checked for these.
    if (gamefile == NULL)
        return EXIT_NOGAMEFILE;
    if (Data == NULL)
        return EXIT_INVALIDPOINTER;

    fseek(gamefile, 0, SEEK_SET);

    char magic[5];
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);
    // magic[4] must be a null since fread() doesn't null terminate.
    magic[4] = '\0';

    if (ret != 4 || strcmp(magic, MAGIC))
        returnInvalid();

    char name[LARGEST_LINE];
    char *value = NULL;
    // Check the BuiltWith value and ensure we aren't opening a version with unknown format.
    // This means BuiltWith MUST be the 2nd line, directly after the magic number
    value = getDataLine(gamefile, name, value);

    if (strcmp(name, "BuiltWith"))
        returnInvalid();

    Data->BuiltWith = atof(value);

    if (Data->BuiltWith > FILES_VERSION)
        returnNew();
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
            if (sscanf(value, "%hu", &Data->NumAttributes) != 1)
                returnInvalid();

            for (short unsigned i = 0; i < Data->NumAttributes; ++i) {
                switch (getAttributeLine(gamefile, name)) {
                case (LINE_ERROR) :
                    returnInvalid();
                case (LINE_START) :
                    --i;
                    break;
                case (LINE_END) :
                    if (i != Data->NumAttributes - 1)
                        returnInvalid();
                    i = LARGEST_ATTRIB_NUM + 1;
                    break;
                case (LINE_SUCCESS) :
                    strcpy(Data->Attributes[i], name);
                }
            }
        }
        if (!strcmp(name, "MapNodes")) {
            short unsigned numnodes;
            if (sscanf(value, "%hu", &numnodes) != 1)
                returnInvalid();

            for (short unsigned i = 0; i < Data->NumAttributes; ++i) {
                switch (getNodeLine(gamefile, &Data->Map[i])) {
                case (LINE_ERROR) :
                    returnInvalid();
                case (LINE_START) :
                    --i;
                    break;
                case (LINE_END) :
                    if (i != numnodes - 1)
                        returnInvalid();
                    i = MAX_MAP_SIZE + 1;
                    break;
                }
            }
        }                    
    }

    return EXIT_SUCCESS;
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
    char *end = (strchr(line, '\n'));
    *end = '\0';
    if (!strcmp(line, "{"))
        return LINE_START;
    if (!strcmp(line, "}"))
        return LINE_END;
    
    return LINE_SUCCESS;
}

static int getNodeLine(FILE *gamefile, struct mapnode *Node)
{
    // Take a MapNode line and insert the data into the appropriate places.
    int ret = fread(Node->Desc, 1, 1, gamefile);
    if (ret == 4)
        return LINE_SUCCESS;
    else
        return LINE_ERROR;
}
