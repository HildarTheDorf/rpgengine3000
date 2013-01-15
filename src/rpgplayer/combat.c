#include "combat.h"

bool combat(const struct datastruct *Data, struct charstruct *Player, unsigned enemyID)
{
    // Dummy enemy.
    struct charstruct Enemy = {"Test Dummy", {1,1,1,1,1,1}};

    puts("Combat!");
    getchar();
    clearScreen();

    printf("%s:\n", Player->Name);
    for (unsigned i = 0; i < Data->NumAttributes; ++i)
        printf("%s: %u\n", Data->Attributes[i], Player->Attributes[i]);

    putchar('\n');

    printf("%s:\n", Enemy.Name);
    for (unsigned i = 0; i < Data->NumAttributes; ++i)
        printf("%s: %u\n", Data->Attributes[i], Enemy.Attributes[i]);

    return true;
}
