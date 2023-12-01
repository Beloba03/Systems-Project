//car_header.h

#define IDLE_CONSUMPTION_RATE 0.001   // kWh per tick for idle mode
#define MOVEMENT_CONSUMPTION_RATE 0.1 // kWh per tick when moving
#define PACKAGE_CONSUMPTION_RATE 0.0001 // kWh per tick per kilogram
#define MAX_CARRY_WEIGHT 2000 // Maximum weight in kilograms
#define INITIAL_CHARGE 100.0 // Initial charge level for cars
#define CHARGING_RATE 0.5   // kWh per tick while charging
#define MAX_CHARGE 100.0     // Maximum charge capacity for cars


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
    float charge;      // Current charge in kWh
    unsigned int carriedWeight; // Weight of the cargo in kilograms
    int isIdle; // 1 if idle, 0 if not idle
    int isCharging; // 1 if charging, 0 if not charging
} Car;