#include "files.h"

int readfile(FILE *gamefile, struct datastruct *Data)
{
    // We should really have already checked for these.
    if (gamefile == NULL)
        return EXIT_NOGAMEFILE;
    if (Data == NULL)
        return EXIT_INVALIDPOINTER;

    fseek(gamefile, 0, SEEK_SET);
    // Check for magic number.
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
        // Simple text fields.
        if (!strcmp(name, "Creator"))
            strncpy(Data->Creator, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Title"))
            strncpy(Data->Title, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Description"))
            strncpy(Data->Description, value, LARGEST_DATA * sizeof(char));
        if (!strcmp(name, "Version"))
            Data->Version = atof(value);

        // Names of the character attributes.
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
            if (getAttributeLine(gamefile, name) != LINE_END)
                returnInvalid();
        }

        // The game map.
        if (!strcmp(name, "MapNodes")) {
            short unsigned numnodes;
            if (sscanf(value, "%hu", &numnodes) != 1)
                returnInvalid();
            if (strcmp(fgets(name, 3, gamefile), "{\n"))
                returnInvalid();
            for (short unsigned i = 0; i < numnodes; ++i) {
                switch (getNodeLine(gamefile, &Data->Map[i], Data)) {
                case (LINE_ERROR) :
                    returnInvalid();
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

static int getNodeLine(FILE *gamefile, struct mapnode *Node, struct datastruct *Data)
{
    // Take a MapNode line and insert the data into the appropriate places.
    short unsigned exitID[EXIT_MAX];
    // Best (ab)use of a scanf format string.
    char format[256];
    sprintf(format, "%%hu=%%hu,%%hu,%%hu,%%hu,%%hu,%%hu,%%hu,%%hu\n%%%d[^\n]\n%%%d[^\n]\n%%%d[^,],%%c\n%%%d[^,],%%c\n%%hx\n",
                        LARGEST_ROOM_NAME, LARGEST_ROOM_DESC, LARGEST_EXIT_NAME, LARGEST_EXIT_NAME);
    int ret = fscanf(gamefile, format,
                        &Node->ID, &exitID[0], &exitID[1], &exitID[2], &exitID[3], &exitID[4], &exitID[5], &exitID[6], &exitID[7],
                        Node->Name, Node->Desc, Node->ExitName[0], &Node->ExitLetter[0], Node->ExitName[1], &Node->ExitLetter[1], &Node->Flags);

    if (ret == 16) {
        Node->ValidExits = 0x0;
        // Convert exits from ID to an actual pointer. Then set ValidExits as appropriate.
        for (short unsigned i = 0; i < EXIT_MAX; ++i) {
            if (exitID[i] != INVALID_ROOM) {
                Node->Exit[i] = &Data->Map[exitID[i]];
                Node->ValidExits |= exit2Flag(i);
            }
        }
        return LINE_SUCCESS;
    }
    else
        return LINE_ERROR;
}
