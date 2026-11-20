#include "Treasure.h"             // common header with all function names

#define ROWS 10                 // the map has 10 rows
#define COLS 10                 // the map has 10 columns

// # How we number the squares
// Every square has a number called a cell: cell = row * 10 + col
// Example: row 2, col 3 is cell 23. To go back: row = cell / 10, col = cell % 10

// # Variables used by the whole game
char **map;                     // the map, created with malloc
int a[MAX][MAX];                // graph as an adjacency matrix (same as Exp 9): a[x][y] = 1 means you can walk from x to y
int v[MAX];                     // visited array for BFS (same as v[] in Exp 9)
int parent[MAX];                // parent[x] = the cell from where BFS reached x
int onpath[MAX];                // onpath[x] = 1 if cell x is on the hint path

int pr, pc;                     // player's row and column
int treasure;                   // cell number of the treasure T
int monster[3];                 // cell numbers of the monsters
int total_monsters;             // how many monsters this level has
int has_key;                    // 1 when the player has picked up the key
int total_moves = 0;            // moves taken in the whole game

// # The 3 levels
// # wall   . road   P player   T treasure   M monster   K key   D locked door
char level1[ROWS][COLS + 1] =
{
    "##########",
    "#P...#...#",
    "#.##.#.#.#",
    "#.#....#.#",
    "#.#.##.#M#",
    "#...#....#",
    "###.#.##.#",
    "#.....#..#",
    "#.###...T#",
    "##########"
};

char level2[ROWS][COLS + 1] =
{
    "##########",
    "#P..##.###",
    "##.#.....#",
    "#...#...M#",
    "#.....M..#",
    "#.#K###..#",
    "##..####.#",
    "#..#.#.#.#",
    "#..D....T#",
    "##########"
};

char level3[ROWS][COLS + 1] =
{
    "##########",
    "#P.......#",
    "##...###.#",
    "#...##M..#",
    "#.#...#..#",
    "###M#K.#.#",
    "##....##.#",
    "##...#.#.#",
    "#M.#.D..T#",
    "##########"
};


// # Create the map with malloc and copy the level into it
void load_level(int level)
{
    int r, c, cell;                                     // row, column, cell number

    map = (char **) malloc(ROWS * sizeof(char *));      // memory for 10 row pointers
    for (r = 0; r < ROWS; r++)
    {
        map[r] = (char *) malloc((COLS + 1) * sizeof(char));   // memory for one row of 10 chars + '\0'
    }

    for (r = 0; r < ROWS; r++)                          // copy every row of the chosen level
    {
        if (level == 1)
            strcpy(map[r], level1[r]);                  // copy row r of level 1
        else if (level == 2)
            strcpy(map[r], level2[r]);                  // copy row r of level 2
        else
            strcpy(map[r], level3[r]);                  // copy row r of level 3
    }

    // # Find the player, the treasure and the monsters on the map
    total_monsters = 0;                                 // no monsters found yet
    has_key = 0;                                        // player starts without the key
    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++)
        {
            cell = r * 10 + c;                          // number of this square
            if (map[r][c] == 'P')                       // player's start
            {
                pr = r;                                 // save row
                pc = c;                                 // save column
                map[r][c] = '.';                        // the square itself is road
            }
            if (map[r][c] == 'T')                       // treasure
                treasure = cell;                        // save where the treasure is
            if (map[r][c] == 'M')                       // a monster
            {
                monster[total_monsters] = cell;         // save where it is
                total_monsters++;                       // one more monster
                map[r][c] = '.';                        // the square itself is road
            }
        }
    }
}

// # Free the map memory when the level is over
void free_map()
{
    int r;                                              // row counter
    for (r = 0; r < ROWS; r++)
    {
        free(map[r]);                                   // free each row
    }
    free(map);                                          // free the array of rows
}

// # Returns 1 if a square can be walked on (not a wall, not a locked door)
int is_open(int r, int c)
{
    if (map[r][c] == '#' || map[r][c] == 'D')
        return 0;                                       // blocked
    else
        return 1;                                       // open
}

// # Build the graph: fill the adjacency matrix a[][]
void build_graph()
{
    int r, c, i, j, cell;                               // row, column, loop counters, cell number

    for (i = 0; i < MAX; i++)                           // first make every entry 0 (no edges)
        for (j = 0; j < MAX; j++)
            a[i][j] = 0;                                // 0 = no edge between i and j

    for (r = 1; r < ROWS - 1; r++)                      // go over every square inside the border
    {
        for (c = 1; c < COLS - 1; c++)
        {
            if (is_open(r, c) == 0)                     // walls and locked doors get no edges
                continue;                               // skip to the next one

            cell = r * 10 + c;                          // number of this square

            if (is_open(r - 1, c)) a[cell][cell - 10] = 1;    // edge to the square above
            if (is_open(r + 1, c)) a[cell][cell + 10] = 1;    // edge to the square below
            if (is_open(r, c - 1)) a[cell][cell - 1] = 1;     // edge to the square on the left
            if (is_open(r, c + 1)) a[cell][cell + 1] = 1;     // edge to the square on the right
        }
    }
}

// # Returns 1 if a monster is standing on this cell
int is_monster(int cell)
{
    int i;                                              // loop counter
    for (i = 0; i < total_monsters; i++)                // check every monster
    {
        if (monster[i] == cell)
            return 1;                                   // found a monster here
    }
    return 0;                                           // no monster here
}

// # Print the map
void display_map(int show_path)
{
    int r, c, cell;                                     // row, column, cell number

    printf("\n");                                       // new line
    for (r = 0; r < ROWS; r++)
    {
        printf("   ");                                  // small space on the left
        for (c = 0; c < COLS; c++)
        {
            cell = r * 10 + c;                          // number of this square
            if (r == pr && c == pc && is_monster(cell))
                printf("X ");                           // a monster caught the player here
            else if (r == pr && c == pc)
                printf("P ");                           // the player
            else if (is_monster(cell))
                printf("M ");                           // a monster
            else if (show_path == 1 && onpath[cell] == 1)
                printf("* ");                           // part of the hint path
            else
                printf("%c ", map[r][c]);               // whatever is on the map (# . T K D)
        }
        printf("\n");                                   // new line
    }
}

// # BFS (same as Exp 9) - we only added parent[] to remember the way back
// If block_monsters is 1, BFS is not allowed to walk through monsters (used for safe hints)
void bfs(int s, int block_monsters)
{
    int src, i;                                         // src = cell taken out of the queue, i = loop counter

    for (i = 0; i < MAX; i++)                           // reset before every BFS
    {
        v[i] = 0;                                       // nothing visited
        parent[i] = -1;                                 // no parent yet
    }

    if (block_monsters == 1)                            // for hints: treat monster squares as visited
    {
        for (i = 0; i < total_monsters; i++)
            v[monster[i]] = 1;                          // so BFS never goes through them
    }

    v[s] = 1;                                           // start cell is visited
    enqueue(s);                                         // put the start cell in the queue

    while (queue_empty() == 0)                          // while the queue has cells
    {
        src = dequeue();                                // take the first cell out

        for (i = 0; i < MAX; i++)                       // check every other cell
        {
            if (a[src][i] == 1 && v[i] == 0)            // there is an edge and i is not visited
            {
                enqueue(i);                             // add i to the queue
                v[i] = 1;                               // mark i visited
                parent[i] = src;                        // we reached i from src
            }
        }
    }
}

// # Move every monster one step towards the player
void move_monsters()
{
    int i;                                              // loop counter

    bfs(pr * 10 + pc, 0);                               // BFS from the PLAYER: now parent[x] is one step closer to the player

    for (i = 0; i < total_monsters; i++)
    {
        if (parent[monster[i]] != -1)                   // the monster can reach the player
            monster[i] = parent[monster[i]];            // so it takes one step towards the player
    }
}

// # Returns 1 if a monster is on the player's square
int caught()
{
    return is_monster(pr * 10 + pc);                    // check the player's cell
}

// # Hint: show a safe path to T using BFS + Stack
void show_hint()
{
    int cell, i, steps = -1;                            // cell number, loop counter, number of steps (start cell is not a step)

    bfs(pr * 10 + pc, 1);                               // BFS from the player, monsters are blocked

    if (v[treasure] == 0)                               // BFS could not reach T
    {
        printf("\nNo safe path right now! Run away from the monsters and try again.\n");  // tell the player
        return;                                         // stop here
    }

    // # parent[] gives the path backwards (T to player), push it on the stack
    cell = treasure;                                    // start from the treasure
    while (cell != -1)                                  // stop after the player's square
    {
        push(cell);                                     // put this square on the stack
        cell = parent[cell];                            // go one step back
    }

    // # Pop from the stack: the path comes out in the right order (player to T)
    for (i = 0; i < MAX; i++)
        onpath[i] = 0;                                  // clear the old path
    while (underflow() == 0)                            // until the stack is empty
    {
        cell = pop();                                   // take the top square
        onpath[cell] = 1;                               // mark it as part of the path
        steps++;                                        // count steps
    }

    display_map(1);                                     // draw the map with * on the path
    printf("\nHint: follow the * path (%d steps). Careful, the monsters move too!\n", steps);  // tell how long the path is
}

// # Play one level
// Returns 1 if the player reaches T, 0 if caught by a monster, -1 if the player quits
int play_level(int level, int lives)
{
    int nr, nc, moves = 0, hints = 3;                   // new row, new col, moves this level, hints left
    char ch;                                            // key pressed by the player

    load_level(level);                                  // create the map
    build_graph();                                      // create the adjacency matrix

    printf("\n################ LEVEL %d ################\n", level);  // level title
    printf("%d monster(s) are chasing you! Reach the treasure T.\n", total_monsters);  // how many monsters
    if (level > 1)
        printf("The door D is locked. Pick up the key K to open it!\n");  // levels 2 and 3 have a key and a door

    while (1)                                           // one turn every time round
    {
        // # Show the map and the status
        display_map(0);                                 // print the map
        printf("\nLives: %d   Moves: %d   Hints: %d", lives, moves, hints);  // print lives, moves and hints
        if (has_key == 1)
            printf("   Key: YES\n");                    // player has the key
        else
            printf("   Key: no\n");                     // player does not have the key
        printf("Move (w a s d), h = hint, q = quit: ");  // ask for a key
        if (scanf(" %c", &ch) != 1)                     // read one key
        {
            free_map();                                 // free the map memory
            return -1;                                  // input ended, quit
        }

        // # Work out the square the player wants to go to
        nr = pr;                                        // new row starts as the current row
        nc = pc;                                        // new column starts as the current column
        if (ch == 'w')
            nr = pr - 1;                                // up
        else if (ch == 's')
            nr = pr + 1;                                // down
        else if (ch == 'a')
            nc = pc - 1;                                // left
        else if (ch == 'd')
            nc = pc + 1;                                // right
        else if (ch == 'h')                             // hint
        {
            if (hints == 0)
                printf("\nNo hints left!\n");           // all 3 hints used
            else
            {
                hints--;                                // use one hint
                show_hint();                            // draw the safe path
            }
            continue;                                   // a hint does not use a move
        }
        else if (ch == 'q')                             // quit
        {
            free_map();                                 // free the map memory
            return -1;                                  // -1 means the player quit
        }
        else
        {
            printf("\nInvalid key. Use w a s d to move.\n");  // any other key
            continue;                                   // go back and ask for the next key
        }

        // # Walls and doors
        if (map[nr][nc] == '#')                         // wall
        {
            printf("\nWall! You can't go there.\n");    // tell the player
            continue;                                   // no move is used
        }
        if (map[nr][nc] == 'D')                         // locked door
        {
            if (has_key == 0)
            {
                printf("\nThe door is locked! Find the key K first.\n");  // no key yet
                continue;                               // go back and ask for the next key
            }
            map[nr][nc] = '.';                          // unlock: the door becomes road
            build_graph();                              // the graph changed, so build it again
            printf("\nYou opened the door!\n");         // tell the player
        }

        // # Move the player
        pr = nr;                                        // player's new row
        pc = nc;                                        // player's new column
        moves++;                                        // one more move in this level
        total_moves++;                                  // one more move in the whole game

        // # Pick up the key
        if (map[pr][pc] == 'K')
        {
            has_key = 1;                                // now the player has the key
            map[pr][pc] = '.';                          // key is removed from the map
            printf("\nYou picked up the KEY! Now you can open the door D.\n");  // tell the player
        }

        // # Did the player walk into a monster?
        if (caught())
        {
            display_map(0);                             // print the map
            free_map();                                 // free the map memory
            return 0;                                   // 0 means caught by a monster
        }

        // # Did the player reach the treasure?
        if (pr * 10 + pc == treasure)
        {
            display_map(0);                             // print the map
            printf("\n*** LEVEL %d CLEARED in %d moves! ***\n", level, moves);  // level won
            free_map();                                 // free the map memory
            return 1;                                   // 1 means the level is cleared
        }

        // # Monsters move after every 2nd move of the player
        if (moves % 2 == 0)
        {
            move_monsters();                            // every monster takes one step towards the player
            if (caught())                               // a monster stepped onto the player
            {
                display_map(0);                         // print the map
                free_map();                             // free the map memory
                return 0;                               // 0 means caught by a monster
            }
        }
    }
}

// # The full game: 3 lives, levels 1 to 3
void play_game()
{
    char name[30];                                      // player's name
    int level = 1, lives = 3, result;                   // start at level 1 with 3 lives

    printf("\nEnter your name: ");                      // ask for the name
    scanf("%29s", name);                                // read the name (one word)
    total_moves = 0;                                    // new game, no moves yet

    while (level <= 3)                                  // until all 3 levels are cleared
    {
        result = play_level(level, lives);              // play this level and get the result

        if (result == 1)                                // level cleared
        {
            level++;                                    // go to the next level
        }
        else if (result == 0)                           // caught by a monster
        {
            lives--;                                    // lose one life
            printf("\nA MONSTER CAUGHT YOU! Lives left: %d\n", lives);  // show lives left
            if (lives == 0)
            {
                printf("\nGAME OVER, %s! Try again.\n", name);  // no lives left
                return;                                 // stop here
            }
            printf("Starting level %d again...\n", level);  // same level again
        }
        else                                            // player quit
        {
            printf("\nYou quit the game.\n");           // player pressed q
            return;                                     // stop here
        }
    }

    // # All levels cleared
    printf("\n************************************\n");
    printf("  %s, YOU FOUND ALL 3 TREASURES!\n", name);
    printf("  Total moves: %d\n", total_moves);
    printf("************************************\n");

    add_score(name, total_moves);                       // put the score in the BST
    show_leaderboard();                                 // show the sorted leaderboard
}
