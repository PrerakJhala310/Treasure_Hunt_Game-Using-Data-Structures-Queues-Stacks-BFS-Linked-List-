#include <stdio.h>      // for printf and scanf
#include <stdlib.h>     // for malloc and free
#include <string.h>     // for strcpy

#define MAX 100         // the map is 10 x 10, so there are 100 cells

// # Functions written in queue.c (Exp 9)
void enqueue(int value);        // add a value at the rear of the queue
int dequeue();                  // remove a value from the front of the queue
int queue_empty();              // returns 1 if the queue is empty

// # Functions written in stack.c (Exp 4)
void push(int value);           // put a value on top of the stack
int pop();                      // take the top value off the stack
int underflow();                // returns 1 if the stack is empty

// # Functions written in bst.c (Exp 8)
void add_score(char name[], int moves);   // insert a player's score in the BST
void show_leaderboard();                  // print all scores in sorted order

// # Functions written in game.c
void play_game();               // runs the full game (level 1, 2 and 3)
