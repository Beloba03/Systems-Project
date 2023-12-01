#pragma once
#include <Windows.h>
#include "linked_list_header.h"
#include <time.h>



// Structure to define properties of a car in the city grid
typedef struct {
    unsigned int x;     // x-position
    unsigned int y;     // y-position
    LinkedList *currentLoc; // Current location
    LinkedList locQueue;    // Queue of locations
    COORD endIntersection;
    COORD endPos;
    unsigned char endIntersectionStatus;
    CarDirection endDirection;
} Car;