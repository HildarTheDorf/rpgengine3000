#include "rpg.h"

int main(int argc, char *argv[])
{
    struct datastruct *Data  = init(argc, argv);

    printf("Load succesful!\n\nTitle: %sCreator: %sVersion: %.1f\nBuilt with version: %.1f\n\nDescription:\n%s\n",
        Data->Title, Data->Creator, Data->Version, Data->BuiltWith, Data->Description);

    for (short unsigned i = 0; i < Data->NumAttributes; ++i)
        printf("Attribute %hu: %s\n", i, Data->Attributes[i]);

    return cleanup(EXIT_SUCCESS, Data);
}

static struct datastruct * init(int argc, char *argv[])
{
#if defined __unix__ || (defined __APPLE__ && defined __MACH__)
    // If not running in a tty, exec to a terminal and restart the program (with --no-auto-exit).
    // Windows kindly does this for us.
    if (!isatty(1))
        spawnTerminal(argc, argv);

    // Check for --no-auto-exit
    if (argc > 2 && (!strcmp(argv[2], "--no-auto-exit")))
        REQUIRE_GETCHAR = true;
    // If arguments given in the wrong order, swap them.
    if (argc > 1 && !strcmp(argv[1], "--no-auto-exit")) {
        char *tmp;
        tmp = argv[2];
        argv[2] = argv[1];
        argv[1] = tmp;
    }
#endif

    clearScreen();
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    // Check for, in order:
    // No arguments, no data file argument, a 'clearly wrong' data file ( from spawnterminal() ), no unknown argv[2], and not too many arguments.
    if (argc < 2 || argv[1] == NULL || !strcmp(argv[1], "/dev/null") || (argc == 3 && strcmp(argv[2], "--no-auto-exit")) || argc > 3) {
        // Check for the last instance of / (or \ on windows). The part after that is the executable name.
        // On windows, don't bother telling the user about --no-auto-exit since it is on by default.
        #ifdef _WIN32
        char *ptr = strrchr(argv[0], '\\');
        printf("Error: invalid arguments\nUsage: %s gamefile\n\n", ptr + 1);
        #else
        char *ptr = strrchr(argv[0], '/');
        printf("Error: invalid arguments\nUsage: %s gamefile [--no-auto-exit]\n\n", ptr + 1);
        #endif

        cleanup(EXIT_ARGUMENTS, NULL);
    }

    char *filename = argv[1];

    printf("Loading game file %s\n", filename);
    
    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        cleanup(EXIT_NOGAMEFILE, NULL);
    }

    // Let's create the main data structure
    struct datastruct *Data = malloc(sizeof(struct datastruct));
    // If malloc fails there's no heap left
    // There's a major (possibly system-wide) error!
    // Panic.
    if (Data == NULL) { 
        puts("\n***CRITICAL ERROR: Could not allocate memory!***\n");
        if (REQUIRE_GETCHAR)
            getchar();
        exit(EXIT_OOM);
    }

    int ret = readfile(gamefile, Data);
    fclose(gamefile);

    if (Data == NULL || ret != EXIT_SUCCESS) {
        cleanup(EXIT_INVALIDGAMEFILE, Data);
    }

    return Data;
}

static int cleanup(int exitstatus, struct datastruct *Data)
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
        return exitstatus;

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
    // Use ANSI terminal escapes to move cursor to top left and wipe the screen.
    puts("\033[H\033[2J");
#endif
    return;
}

static void spawnTerminal(int argc, char *argv[])
{
    // We are not running in a (pseudo-)terminal, let's try to launch one:
    if (getenv("DISPLAY") == NULL)
        // No controlling terminal, and we are not within an X server.
        // Therefore we are running as a daemon?!
        // Panic.
        exit(EXIT_NOTINTERACTIVE);

    char* term = getenv("TERM");
    // If we didn't get a TERM, assume xterm for now, we will try others later.
    if (term == NULL)
        term = "xterm";

    // Set a clearly wrong file name for argv[1] if we didn't get one.
    if (argc == 1)
        argv[1] = "/dev/null";

    // The real value for argmax would be in the millions through sysconf(_SC_ARG_MAX), but we can't trust it fully because it can clobber environment variables.
    // Halving it is a 'reasonable' failsafe.
    long argmax = sysconf(_SC_ARG_MAX) / 2;
    char commandline[argmax];

    // 35 = extra overhead in the commandline for the terminal name, and other parts of the (new) command line.
    if (strlen(argv[0]) + strlen(argv[1]) + 35 > argmax)
        // We can't fit our extra arguments into a command line.
        // Panic.
        exit(EXIT_ARGUMENTS);

    // Make the command line for the new program up. Add --no-auto-exit since the terminal will close on program completion, windows style.
    sprintf(commandline, "%s %s --no-auto-exit", argv[0], argv[1]);

    // Try some common "pretty" terminals rather than the ugly xterm, which often ends up the 'default'.
    if (!strcmp(term, "xterm")) {
        execlp("gnome-terminal", "gnome-terminal", "-e", commandline, NULL);
        execlp("konsole", "konsole", "-e", commandline, NULL);
        execlp("urxvt", "urxvt", "-e", commandline, NULL);
    }
    execlp(term, term, "-e", commandline, NULL);
    // $TERM does not point to a valid terminal, or exec failed. One final attempt to launch one.
    execlp("xterm", "xterm", "-e", commandline, NULL);
    // $TERM does not point to a valid terminal and xterm is not installed, or we can't exec for some reason.
    // We can't even report this for obvious reasons.
    // Give up.
    exit(EXIT_NOTINTERACTIVE);
}
