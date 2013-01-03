// Contains main(), as well as init and exit routines.
#include "main.h"

int main(int argc, char *argv[])
{
    initGlobal(argc, argv);  //Process the commandline and re-exec in a terminal if required.

    struct datastruct *Data  = smalloc(sizeof(*Data));
    initData(argv[1], Data); //Open up the datafile and place its data to the Data struct.

    struct mapnode *Location; //Get a pointer to map node ID 0. Eventually recall the last location from a savefile.
    initMap(Data, &Location);

    struct charstruct *Player = smalloc(sizeof(*Player));
    initPlayer(Data, Player); // Eventually load a file, right now puts dummy values in a player array.

    printf("Load succesful!\n\nTitle: %sCreator: %sVersion: %.1f\nBuilt with version: %.1f\n\nDescription:\n%s\n",
        Data->Title, Data->Creator, Data->Version, Data->BuiltWith, Data->Description);

    printf("Player: Attributes %d, %d, %d, %d, %d, %d.\nName: %s\n",
        Player->Attributes[0], Player->Attributes[1], Player->Attributes[2], Player->Attributes[3], Player->Attributes[4], Player->Attributes[5], Player->Name);

    puts("Press enter to begin...");
    flushchar('\0');

    int ret = mainLoop(Data, Location, Player);

    return cleanup(ret, Data, Player);
}

static void initGlobal(int argc, char *argv[])
{
#if defined _WIN32
    setGetchar(1);
#endif
#if defined __unix__ || (defined __APPLE__ && defined __MACH__)
    // If arguments given in the wrong order, swap them.
    if (argc > 1 && !strcmp(argv[1], "--no-auto-exit")) {
        char *tmp;
        tmp = argv[2];
        argv[2] = argv[1];
        argv[1] = tmp;
    }
    // If not running in a tty, exec to a terminal and restart the program (with --no-auto-exit).
    // Windows kindly opens a terminal for us. (1 == stdout)
    if (!isatty(1))
        spawnTerminal(argc, argv);
    // Check for --no-auto-exit
    if (argv[2] != NULL && (!strcmp(argv[2], "--no-auto-exit")))
        setGetchar(1);

#endif

    clearScreen();
    printf("rpgengine3000 version %.1f\nThis product is free software released under the GPL v3\nFor more information see the file COPYING\n\n", PLAYER_VERSION);

    // Check for, in order:
    // No arguments, no data file argument, a 'clearly wrong' data file ( from spawnterminal() ), an unknown argv[2], and too many arguments.
    if (argc < 2 || argv[1] == NULL || !strcmp(argv[1], "/dev/null") || (argc == 3 && strcmp(argv[2], "--no-auto-exit")) || argc > 3) {
        // Check for the last instance of / (or \ on windows) in argv[0]. The part after that is the executable name.
        // On windows, don't bother telling the user about --no-auto-exit since it is implicitly always on.
        #if defined __unix__ || (defined __APPLE__ && __MACH__)
        char *ptr = strrchr(argv[0], '/');
        fprintf(stderr, "Error: invalid arguments\nUsage: %s gamefile [--no-auto-exit]\n\n", ptr + 1);
        #else
        char *ptr = strrchr(argv[0], '\\');
        fprintf(stderr, "Error: invalid arguments\nUsage: %s gamefile\n\n", ptr + 1);
        #endif

        cleanup(EXIT_ARGUMENTS, NULL, NULL);

        return;
    }
}

static void initData(char *filename, struct datastruct *Data)
{
    printf("Loading game file %s\n", filename);
    
    FILE *gamefile = fopen(filename, "r");

    if (gamefile == NULL) {
        printf("Game file %s not found.\n", filename);
        cleanup(EXIT_NOGAMEFILE, Data, NULL);
    }

    int ret = readfile(gamefile, Data);
    fclose(gamefile);

    if (Data == NULL || ret != EXIT_SUCCESS) {
        cleanup(EXIT_INVALIDGAMEFILE, Data, NULL);
    }

    return;
}

static void initMap(struct datastruct *Data, struct mapnode **Location)
{
    *Location = &Data->Map[0];
}

static void initPlayer(const struct datastruct *Data, struct charstruct *Player)
{
    for (unsigned short i = 0; i < Data->NumAttributes; ++i)
        Player->Attributes[i] = 1;
    strcpy(Player->Name, "Shalune");
}

static int cleanup(int exitstatus, struct datastruct *Data, struct charstruct *Player)
{
    if (Data != NULL) {
        free(Data);
        Data = NULL;
    }

    if (Player != NULL) {
        free(Player);
        Player = NULL;
    }

    if (requireGetchar() && exitstatus != EXIT_SUCCESS) {
        puts("Press any key to continue...\n");
        getchar();
    }
    if (exitstatus == EXIT_SUCCESS)
        return exitstatus;

    exit(exitstatus);
}

void clearScreen(void)
{
#ifdef _WIN32
    /* Standard error macro for reporting Win32 API errors */
    #define PERR(bSuccess, api){if(!(bSuccess)) printf("WinAPI error in %s:Error %d from %s \
    on line %d\n", __FILE__, GetLastError(), api, __LINE__);}
    
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the cursor */
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in the current buffer */

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

#if defined __unix__ || (defined __APPLE__ && defined __MACH__)
#include <syslog.h>
static void spawnTerminal(int argc, char *argv[])
{
    char* term = getenv("TERM");
    // If we didn't get a TERM (likely, since we aren't already in one), assume xterm for now, we will try others later.
    if (term == NULL)
        term = "konsole";

    // Set a clearly wrong file name for argv[1] if we didn't get one.
    if (argc == 1 || !strcmp(argv[1], ""))
        argv[1] = "/dev/null";

    // The real value for arg max is normally in the millions through sysconf(_SC_ARG_MAX), at at least 4096.
    // But we can't trust it fully because it includes, and can thus we could clobber, environment variables.
    // Halving it is a 'reasonable' failsafe.
    long unsigned argmax = sysconf(_SC_ARG_MAX) / 2;
    char commandline[argmax];

    // 50 = extra overhead in the commandline for the terminal name, and other parts of the (new) command line.
    if (strlen(argv[0]) + strlen(argv[1]) + 50 > argmax)
        // We can't fit our extra arguments into a command line.
        // Panic.
        exit(EXIT_ARGUMENTS);


    // New command line #define for multi-string argument, single string for gnome-terminal...
    #define COMMANDLINE argv[0], argv[1], "--no-auto-exit"
    sprintf(commandline, "%s %s --no-auto-exit", argv[0], argv[1]);


    // OSX Terminal. Be nice if someone could tell me if this actually works.
    #if defined __APPLE__ && defined __MACH__
    execlp("open", "open", "-a", "/Applications/Terminal.app", COMMANDLINE, NULL);
    #endif
    

    // Try some common "pretty" terminals rather than the ugly xterm(and it's brethren such as xterm-color), which often end up 'default' for hysterical raisins.
    // gnome-terminal insists on a single string for arguments to -e, while konsole insists on separate null-terminated strings...
    if (!strcmp(term, "xterm")) {
        execlp("gnome-terminal", "gnome-terminal", "-e", commandline, NULL);
        execlp("konsole", "konsole", "-e", COMMANDLINE, NULL);
        execlp("urxvt", "urxvt", "-e", COMMANDLINE, NULL);
    }

    // gnome-terminal is special.
    if (!strcmp(term, "gnome-terminal"))
        execlp(term, term, "-e", commandline, NULL);

    // konsole, xterm, etc.
    execlp(term, term, "-e", COMMANDLINE, NULL);

    // $TERM does not point to a valid terminal (or exec failed). One final attempt to launch one.
    execlp("xterm", "xterm", "-e", COMMANDLINE, NULL);

    // Everything failed.
    // Report to syslog and give up.
    syslog(LOG_MAKEPRI(LOG_USER, LOG_ERR),
        "<error> Could not spawn terminal for use.");
    syslog(LOG_MAKEPRI(LOG_USER, LOG_ERR),
        "<error> Check $PATH is set correctly, a terminal emulator such as xterm is installed, or run from a tty.");
    exit(EXIT_NOTINTERACTIVE);
}
#endif

bool setGetchar(bool set)
{
    requiregetchar = set;
    return requiregetchar;
}

bool requireGetchar(void)
{
    return requiregetchar;
}
