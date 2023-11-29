#pragma once
#include <Windows.h>
// Enumeration to define possible car directions
typedef enum CarDirection{
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

// Structure to define a location
typedef struct location {
    COORD endPos;
    CarDirection endDir;
} location;

typedef struct LinkedList
{
    location loc;
    struct LinkedList *next;
} LinkedList;

// Structure to define properties of a car in the city grid
typedef struct {
    unsigned int x;     // x-position
    unsigned int y;     // y-position
    LinkedList *currentLoc; // Current location
    LinkedList locQueue;    // Queue of locations
    COORD endIntersection;
    COORD endPos;
    unsigned char endIntersectionStatus;
} Car;