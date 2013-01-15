#include "rpg.h"

// Location gets passed around as a (struct mapnode **) so that deeply nested functions can change the location.
// If a function has no need to modify a struct it gets given a (const struct structname *). ((Even Location)).

int mainLoop(const struct datastruct *Data, struct mapnode *Location, struct charstruct *Player)
{   
    bool ret = false;
    srand(1); // Initilize the random seed, or rather, don't while debugging.
    printDescription(Location);

    while (ret == false) { // If any function returns true, ret is set to true and we exit.
        ret |= handleInput(Data, &Location, Player);
        ret |= randomEvent(Data, &Location, Player);
    }

    return EXIT_SUCCESS;
}

static bool handleInput(const struct datastruct *Data, struct mapnode **Location, struct charstruct *Player)
{
    char input = getchar();
    flushchar(input);
    input = tolower(input);

    switch (input) {
    case 'q' :
        return true;
    case 'l' :
        printDescription(*Location);
        break;
    case 'n' :
        movePlayer(Location, EXIT_NORTH);
        break;
    case 's' :
        movePlayer(Location, EXIT_SOUTH);
        break;
    case 'e' :
        movePlayer(Location, EXIT_EAST);
        break;
    case 'w' :
        movePlayer(Location, EXIT_WEST);
        break;
    case 'u' :
        movePlayer(Location, EXIT_UP);
        break;
    case 'd' :
        movePlayer(Location, EXIT_DOWN);
        break;
    }
    for (unsigned short i = EXIT_EXTRA0; i < EXIT_MAX; ++i)
        if (input == (*Location)->ExitLetter[exit2ID(i)])
            movePlayer(Location, i);

    return false;
}

static void printDescription(const struct mapnode *Location)
{
    clearScreen();
    printf("%s\n\n%s\nExits:\n", Location->Name, Location->Desc);
    if (Location->ValidExits & NORTH_VALID)
        printf("North<N> ");
    if (Location->ValidExits & SOUTH_VALID)
        printf("South<S> ");
    if (Location->ValidExits & EAST_VALID)
        printf("East<E> ");
    if (Location->ValidExits & WEST_VALID)
        printf("West<W> ");
    if (Location->ValidExits & UP_VALID)
        printf("Up<U> ");
    if (Location->ValidExits & DOWN_VALID)
        printf("Down<D> ");
    for (unsigned short i = EXIT_EXTRA0; i < EXIT_MAX; ++i)
        if (Location->ValidExits & exit2Flag(i))
            printf("%s<%c> ", Location->ExitName[exit2ID(i)], toupper(Location->ExitLetter[exit2ID(i)]));

    puts("\n");
}

static void movePlayer(struct mapnode **Location, short direction)
{
    int exitflag = exit2Flag(direction);
    if ((*Location)->ValidExits & exitflag) {
        *Location = (*Location)->Exit[direction];
        printDescription(*Location);
    }
    return;
}

static bool randomEvent(const struct datastruct *Data, struct mapnode **Location, struct charstruct *Player)
{
    double random = (double)rand()/RAND_MAX;
    if (random < 0.5)
        return combat(Data, Player, 1);
    return false;
}
