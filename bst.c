#include "doodle.h"             // common header with all function names

// # BST node (same as Exp 8) - one node stores one player's score
struct Node
{
    char name[30];              // player's name
    int data;                   // total moves taken (smaller is better)
    struct Node *left;          // players with fewer moves
    struct Node *right;         // players with more (or equal) moves
} *root = NULL;                 // root of the tree, empty at the start

// # Create a new node with malloc
struct Node *create()
{
    struct Node *ptr = (struct Node *) malloc(sizeof(struct Node));   // get memory for one node
    ptr->left = NULL;           // no left child yet
    ptr->right = NULL;          // no right child yet
    return ptr;                 // give back the new node
}

// # Insert a score in the BST (recursive, same as Exp 8)
struct Node *insert(struct Node *root, char name[], int value)
{
    if (root == NULL)                           // found an empty place
    {
        struct Node *newnode = create();        // make a new node
        strcpy(newnode->name, name);            // save the name
        newnode->data = value;                  // save the moves
        return newnode;                         // this node is placed here
    }

    if (value < root->data)                     // fewer moves
    {
        root->left = insert(root->left, name, value);     // go to the left side
    }
    else                                        // more or equal moves
    {
        root->right = insert(root->right, name, value);   // go to the right side
    }

    return root;                                // return the unchanged root
}

// # Inorder traversal (Left, Root, Right) - prints players from fewest moves to most
void inorder(struct Node *root)
{
    if (root != NULL)                           // stop when there is no node
    {
        inorder(root->left);                    // first print the left side
        printf("  %-15s %d moves\n", root->name, root->data);   // then this node
        inorder(root->right);                   // then the right side
    }
}

// # Count how many nodes are in the tree (same as Exp 8)
int countnodes(struct Node *root)
{
    int lc, rc, total;                          // left count, right count, total

    if (root == NULL)                           // empty tree has 0 nodes
    {
        return 0;
    }

    lc = countnodes(root->left);                // count nodes on the left
    rc = countnodes(root->right);               // count nodes on the right

    total = 1 + lc + rc;                        // this node + left + right

    return total;
}

// # Find the node with the smallest value (keep going left, same as Exp 8)
struct Node *findMinimum(struct Node *root)
{
    while (root != NULL && root->left != NULL)  // while there is a left child
    {
        root = root->left;                      // move left
    }
    return root;                                // leftmost node = fewest moves
}

// # Add a finished player to the leaderboard
void add_score(char name[], int moves)
{
    root = insert(root, name, moves);           // insert into the BST
}

// # Print the leaderboard
void show_leaderboard()
{
    struct Node *best;                          // will point to the best player

    if (root == NULL)                           // no one has finished yet
    {
        printf("\nNo scores yet. Finish all 3 levels to get on the leaderboard!\n");
        return;
    }

    printf("\n######## LEADERBOARD ########\n");
    inorder(root);                              // sorted list using inorder

    best = findMinimum(root);                   // leftmost node is the winner
    printf("\nBest player : %s (%d moves)\n", best->name, best->data);
    printf("Total players : %d\n", countnodes(root));    // number of nodes
}
