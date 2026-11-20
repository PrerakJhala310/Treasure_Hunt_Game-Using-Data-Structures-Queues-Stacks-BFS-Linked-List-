#include "Treasure.h"             // common header with all function names

// # Stack (same as Exp 4) - used to turn the hint path around (LIFO)
int stk[MAX];                   // array that stores the stack
int top = -1;                   // -1 means the stack is empty

// # Check if the stack is full
int overflow()
{
    if (top == MAX - 1)         // top is at the last index
        return 1;               // full
    else
        return 0;               // not full
}

// # Check if the stack is empty
int underflow()
{
    if (top == -1)              // top is -1
        return 1;               // empty
    else
        return 0;               // not empty
}

// # Put a value on top of the stack
void push(int value)
{
    int k;                      // stores the result of overflow()
    k = overflow();             // check if the stack is full
    if (k == 1)
    {
        printf("Stack is full\n");      // cannot push
    }
    else
    {
        top++;                  // move top up by one
        stk[top] = value;       // store the value at the new top
    }
}

// # Take the top value off the stack
int pop()
{
    int value;                  // to hold the popped value
    int k;                      // stores the result of underflow()
    k = underflow();            // check if the stack is empty
    if (k == 1)
    {
        printf("Stack is Empty\n");     // nothing to pop
        return -1;              // return -1 to show failure
    }
    else
    {
        value = stk[top];       // read the top value
        top--;                  // move top down by one
        return value;           // give back the value
    }
}
