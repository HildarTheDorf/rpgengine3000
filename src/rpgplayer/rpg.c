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

    printf("Load succesful!\n\nTitle: %sCreator: %sVersion: %.1f\nBuilt with rpgcreator3000 version: %.1f\nDescription:\n%s\n", Data->Title, Data->Creator, Data->Version, Data->BuiltWith, Data->Description);

    cleanup(EXIT_SUCCESS, Data);

    return EXIT_NEVEROCCUR; //This should never occur!
}

static struct datastruct * init(char* filename)
{
    printf("Loading game file %s...\n", filename);

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
        exit(EXIT_INVALIDGAMEFILE);
    }

    return Data;
}

static void cleanup(int exitstatus, struct datastruct *Data)
{
    if (Data != NULL)
        free(Data);
    exit(exitstatus);
}

static void clearScreen(void)
{
#if defined _WIN32

/* Untested! */
#include <windows.h>

void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
#elif defined __linux__ || defined TARGET_OS_MAC

    /* Use ANSI terminal escapes to move cursor to top left and wipe the screen,  */
    /* Windows are you even trying? */
    printf("\033[H\033[2J");

#else
    #error "Unknown Platform. Please compile on Linux, MacOS X or Windows."
#endif
    return;
}
