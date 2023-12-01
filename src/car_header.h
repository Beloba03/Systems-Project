#pragma once
#include <Windows.h>
#include "linked_list_header.h"
#include <time.h>


// Struct to define save data
typedef struct {
    int vin;
    char lastStable[4];
    char lastStableQuad[4];
    int batteryCapacity;
    int batteryCharge;
    int batteryChargeRate;
    int drivingDischargeRate;
    int idleDischargeRate;
    int moveTime;
    int idleTime;
    int chargeTime;
} VehicleRecord;

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
    VehicleRecord vehicleRecord;
} Car;

extern void copyToMainBuffer();