#include "rpg.h"

int main(int argc, char *argv[])
{
    clearScreen();
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    if (argc == 1) {
        printf("Error: insufficent arguments\nUsage: %s gamefile\n\n", argv[0]);
        cleanup(EXIT_ARGUMENTS, NULL);
    }

    struct datastruct *Data  = init(argv[1]);

    if (Data == NULL)
        cleanup(EXIT_INVALIDGAMEFILE, NULL);

    printf("Load succesful!\n\nTitle: %sCreator: %sVersion: %.1f\nBuilt with version: %.1f\n\nDescription:\n%s\n",
        Data->Title, Data->Creator, Data->Version, Data->BuiltWith, Data->Description);

    cleanup(EXIT_SUCCESS, Data);

    return EXIT_NEVEROCCUR; // This should never occur! ^^
}

static struct datastruct * init(char* filename)
{
    printf("Loading game file %s\n", filename);

    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        cleanup(EXIT_NOGAMEFILE, NULL);
    }

    char magic[5] = "\0\0\0\0\0";
    int ret;
    ret = fread(magic, sizeof(char), 4, gamefile);

    if (ret != 4 || strcmp(magic, MAGIC)) {
        printf("Game file %s invalid or corrupt\n", filename);
        fclose(gamefile);
        cleanup(EXIT_INVALIDGAMEFILE, NULL);
    }

    struct datastruct *Data = readfile(gamefile);
    fclose(gamefile);

    if (Data == NULL) {
        cleanup(EXIT_INVALIDGAMEFILE, NULL);
    }

    return Data;
}

static void cleanup(int exitstatus, struct datastruct *Data)
{
    if (Data != NULL) 
        free(Data);

#   ifdef _WIN32
        puts("Press any key to continue...");
        getchar();
#   endif

    exit(exitstatus);
}

static void clearScreen(void)
{
#ifdef _WIN32
    // Windows' use of system(), considered harmful.
    system("CLS");
#elif defined __unix__ || (defined __APPLE__ && defined __MACH__)
    // Use ANSI terminal escapes to move cursor to top left and wipe the screen
    printf("\033[H\033[2J");
#else
#   error "Please compile on windows or a POSIX compliant system."
#endif
    return;
}
