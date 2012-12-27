#include "rpg.h"

int mainLoop(struct datastruct Data, struct mapnode *Location)
{   
    printDescription(*Location);

    while (handleInput(Data, &Location) == false)
        ;

    return EXIT_SUCCESS;
}

static void printDescription(struct mapnode Location)
{
    clearScreen();
    printf("%s\n\n%s\nExits:\n", Location.Name, Location.Desc);
    if (Location.ValidExits &= NORTH_VALID)
        puts("North <N> ");
    if (Location.ValidExits &= SOUTH_VALID)
        puts("South <S> ");
    if (Location.ValidExits &= EAST_VALID)
        puts("East <E> ");
    if (Location.ValidExits &= WEST_VALID)
        puts("West <W> ");
    if (Location.ValidExits &= UP_VALID)
        puts("Up <U> ");
    if (Location.ValidExits &= DOWN_VALID)
        puts("Down <D> ");
    if (Location.ValidExits &= EXTRA0_VALID)
        printf("%s <%c> ", Location.ExitName[0], Location.ExitLetter[0]);
    if (Location.ValidExits &= EXTRA1_VALID)
        printf("%s <%c> ", Location.ExitName[1], Location.ExitLetter[1]);


}

static bool handleInput(struct datastruct Data, struct mapnode **Location)
{
    char input = getchar();
    flushchar(input);
    input = tolower(input);
    switch (input) {
    case 'q' :
        return true;
    case 'l' :
        printDescription(**Location);
        break;
    case 'n' :
    case 's' :
    case 'e' :
    case 'w' :
    case 'u' :
    case 'd' :
        break;
    }
    return false;
}
