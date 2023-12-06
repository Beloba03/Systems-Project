#pragma once
#include <Windows.h>
#include "linked_list_header.h"
#include <time.h>

#define MAX_VIN_COUNT 10


// Struct to define save data
typedef struct {
    int vin;
    char lastStable[4];
    char lastStableQuad[4];
    unsigned int batteryCapacity;
    unsigned int batteryCharge;
    unsigned int batteryChargeRate;
    unsigned int drivingDischargeRate;
    unsigned int idleDischargeRate;
    unsigned int moveTime;
    unsigned int idleTime;
    unsigned int chargeTime;
} VehicleRecord;

// Structure to define properties of a car in the city grid
typedef struct {
    unsigned int x;     // x-position
    unsigned int y;     // y-position
    unsigned int endFloor; // Floor of the destination
    LinkedList locQueue;    // Queue of locations
    unsigned int delTime; // Current Delivery Time
    COORD endIntersection; // Intersection of the destination
    COORD endPos; // Destination
    unsigned char endIntersectionStatus; // Car's status
    CarDirection endDirection; // How to move to enter parking spot
    VehicleRecord vehicleRecord; // Vehicle information 
} Car;

// Structure to define a delivery request
typedef struct {
    int time;
    char event;
    int origin_customer_id;
    int destination_customer_id;
    float package_weight;
} EventRecord;


extern void copyToMainBuffer();
extern EventRecord getCurrentEvent(int reset);
extern location getCustDest(int custID);
extern int tickTime;
extern void saveDelInfo(int carNum);