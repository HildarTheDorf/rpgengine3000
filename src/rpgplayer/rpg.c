#include "rpg.h"
#ifdef _WIN32
#   include <windows.h>
#endif

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

    for (short unsigned i = 0; i < Data->NumAttributes; ++i)
        printf("Attribute %hu: %s\n", i, Data->Attributes[i]);

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
    PERR( bSuccess, "GetConsoleScreenBufferInfo" );
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
