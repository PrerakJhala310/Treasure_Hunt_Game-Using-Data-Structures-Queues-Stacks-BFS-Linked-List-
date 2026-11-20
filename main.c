#include "Treasure.h"             // common header with all function names

int main()
{
    int choice;                 // menu choice typed by the user

    printf("\n######## DOODLE MAPS : ESCAPE THE MONSTERS ########\n");

    do
    {
        // # Show the menu
        printf("\n1. Play game\n");                     // menu option 1
        printf("2. How to play\n");                     // menu option 2
        printf("3. Leaderboard\n");                     // menu option 3
        printf("4. Exit\n");                            // menu option 4
        printf("Enter choice: ");                       // ask for a choice

        // # Read the choice (if a letter is typed, skip it)
        if (scanf("%d", &choice) != 1)
        {
            if (feof(stdin))            // no more input, stop the program
                break;                  // leave the menu loop
            scanf("%*s");               // throw away the wrong input
            choice = 0;                 // this will print "Invalid choice"
        }

        // # Do what the user chose
        switch (choice)
        {
            case 1:
                play_game();            // start the game
                break;                                  // leave the switch

            case 2:
                printf("\nHOW TO PLAY\n");              // rules title
                printf("- You are P. Reach the treasure T on each level.\n");
                printf("- Move with w (up), s (down), a (left), d (right), then press Enter.\n");
                printf("- Monsters M chase you. They move after every 2 moves you make.\n");
                printf("- Level 1 has 1 monster, Level 2 has 2, Level 3 has 3.\n");
                printf("- D is a locked door. Pick up the key K to open it.\n");
                printf("- h shows a safe path (3 hints per level).\n");
                printf("- You have 3 lives. Fewest total moves tops the leaderboard!\n");
                break;                                  // leave the switch

            case 3:
                show_leaderboard();     // print the BST leaderboard
                break;                                  // leave the switch

            case 4:
                printf("Exiting...\n");                 // goodbye message
                break;                                  // leave the switch

            default:
                printf("Invalid choice\n");             // any other number
        }
    } while (choice != 4);              // keep showing the menu until 4 is chosen

    return 0;                                           // program ends
}
