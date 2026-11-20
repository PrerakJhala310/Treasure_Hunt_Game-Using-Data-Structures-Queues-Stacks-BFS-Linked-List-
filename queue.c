#include "doodle.h"             // common header with all function names

// # Queue (same as Exp 9) - used by BFS to visit cells in the order they are found (FIFO)
int queue[MAX];                 // array that stores the queue
int front = -1, rear = -1;      // -1 means the queue is empty

// # Add a value at the rear of the queue
void enqueue(int value)
{
    if (rear == MAX - 1)                // rear is at the last index, so the queue is full
    {
        printf("Queue Overflow\n");     // tell the user it is full
        return;                         // stop, nothing is added
    }

    if (front == -1)                    // queue was empty
        front = 0;                      // front now points to the first element

    rear++;                             // move rear one step ahead
    queue[rear] = value;                // store the value at rear
}

// # Remove a value from the front of the queue
int dequeue()
{
    int value;                          // to hold the removed value
    if (front == -1)                    // nothing in the queue
    {
        printf("Queue Underflow\n");    // tell the user it is empty
        return -1;                      // return -1 to show failure
    }
    value = queue[front];               // take the value at front
    if (front == rear)                  // that was the only element
    {
        front = -1;                     // queue becomes empty again
        rear = -1;
    }
    else
        front++;                        // move front to the next element

    return value;                       // give back the removed value
}

// # Check if the queue is empty (same check as while(front!=-1 && rear!=-1) in Exp 9)
int queue_empty()
{
    if (front == -1 && rear == -1)      // both -1 means empty
        return 1;                       // yes, empty
    else
        return 0;                       // no, it has elements
}
