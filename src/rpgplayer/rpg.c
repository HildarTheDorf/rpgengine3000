#include "rpg.h"

int main(int argc, char *argv[])
{
    struct datastruct *Data  = init(argc, argv);

    printf("Load succesful!\n\nTitle: %sCreator: %sVersion: %.1f\nBuilt with version: %.1f\n\nDescription:\n%s\n",
        Data->Title, Data->Creator, Data->Version, Data->BuiltWith, Data->Description);

    for (short unsigned i = 0; i < Data->NumAttributes; ++i)
        printf("Attribute %hu: %s\n", i, Data->Attributes[i]);

    cleanup(EXIT_SUCCESS, Data);
    return EXIT_SUCCESS;
}

static struct datastruct * init(int argc, char *argv[])
{
#if defined __unix__ || (defined __APPLE__ && defined __MACH__)
    if (!isatty(1))
        spawnTerminal(argc, argv);
#endif

    char *filename = argv[1];
    if (argc > 2 && !strcmp(argv[2], "y"))
        REQUIRE_GETCHAR = true;

    clearScreen();
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    if (argc < 2 || !strcmp(argv[1], "/dev/null") || (argc == 3 && *argv[2] != 'y') || argc > 3) {
        #ifdef _WIN32
        char *ptr = strrchr(argv[0], '\\');
        #else
        char *ptr = strrchr(argv[0], '/');
        #endif
        printf("Error: invalid arguments\nUsage: %s gamefile\n\n", ptr + 1);
        cleanup(EXIT_ARGUMENTS, NULL);
    }
    printf("Loading game file %s\n", filename);
    
    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        cleanup(EXIT_NOGAMEFILE, NULL);
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
    if (Data != NULL) {
        free(Data);
        Data = NULL;
    }

    if (REQUIRE_GETCHAR) {
        puts("Press any key to continue...");
        getchar();
    }
    if (exitstatus == EXIT_SUCCESS)
        return;

    exit(exitstatus);
}

static void clearScreen(void)
{
#ifdef _WIN32
    /* Standard error macro for reporting Win32 API errors */
    #define PERR(bSuccess, api){if(!(bSuccess)) printf("%s:Error %d from %s \
    on line %d\n", __FILE__, GetLastError(), api, __LINE__);}
    
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in
                                        the current buffer */

    /* get the number of character cells in the current buffer */
    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    PERR(bSuccess, "GetConsoleScreenBufferInfo" );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter( hConsole, (TCHAR) ' ',
       dwConSize, coordScreen, &cCharsWritten );
    PERR( bSuccess, "FillConsoleOutputCharacter" );

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
    PERR( bSuccess, "ConsoleScreenBufferInfo" );

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute( hConsole, csbi.wAttributes,
       dwConSize, coordScreen, &cCharsWritten );
    PERR( bSuccess, "FillConsoleOutputAttribute" );

    /* put the cursor at (0, 0) */

    bSuccess = SetConsoleCursorPosition( hConsole, coordScreen );
    PERR( bSuccess, "SetConsoleCursorPosition" );
#elif defined __unix__ || (defined __APPLE__ && defined __MACH__)
    // Use ANSI terminal escapes to move cursor to top left and wipe the screen
    // Windows, are you even trying?
    puts("\033[H\033[2J");
#else
#   error "Please compile on windows or a POSIX compliant system."
#endif
    return;
}

static void spawnTerminal(int argc, char *argv[])
{
        // We are not running in a (pseudo-)terminal, let's try to launch one:
        if (getenv("DISPLAY") == NULL)
            // We are running as a daemon...
            // Panic.
            exit(EXIT_NOTINTERACTIVE);

        char* term = getenv("TERM");
        // If we didn't get a TERM, assume xterm for now, we will try other later
        if (term == NULL)
            term = "xterm";

        // Set a clearly wrong file name for argv[1] if we didn't get one
        if (argc == 1)
            argv[1] = "/dev/null";

        // Make the command line for the new program up
        char commandline[4096]; //4096 is the minimum posix max argument length. Most implementations are far larger.
        for (unsigned short i = 0; i < 4096; ++i)
            commandline[i] = '\0';
        strcpy(&commandline[0], argv[0]);
        strcpy(strchr(commandline, '\0'), " ");
        strcpy(strchr(commandline, '\0'), argv[1]);
        strcpy(strchr(commandline, '\0'), " y");

        // Try some common "pretty" terminals rather than the ugly xterm, which often ends up the 'default'.
        if (!strcmp(term, "xterm")) {
            execlp("gnome-terminal", "gnome-terminal", "-e", commandline, NULL);
            execlp("konsole", "konsole", "-e", argv[0], argv[1], "y", NULL);
            execlp("urxvt", "urxvt", "-e", argv[0], argv[1], "y", NULL);
            execlp("rxvt", "rxvt", "-e", argv[0], argv[1], "y", NULL);
        }
        execlp(term, term, "-e", argv[0], argv[1], "y", NULL);
        // $TERM does not point to a valid terminal, and no common ones found
        // Panic.
        exit(EXIT_NOTINTERACTIVE);
}
