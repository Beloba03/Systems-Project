#pragma once
#include <Windows.h>
// Enumeration to define possible car directions
typedef enum CarDirection{
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT,
    STOP
} CarDirection;

// Structure to define a location
typedef struct location {
    COORD endPos;
    CarDirection endDir;
    int floorNum;
    int time;
} location;

// Structure to define a linked list
typedef struct LinkedList
{
    location loc;
    struct LinkedList *next;
} LinkedList;

extern void enqueue(int carNum, location loc);
extern location dequeue(int carNum);
extern location peek(int carNum, int locationPos);
extern void printQueue(int carNum);