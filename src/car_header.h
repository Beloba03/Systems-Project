#pragma once
#include <Windows.h>
#include "linked_list_header.h"
#include <time.h>


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
    unsigned int endFloor;
    LinkedList locQueue;    // Queue of locations
    unsigned int delTime; // Current Delivery Time
    COORD endIntersection;
    COORD endPos;
    unsigned char endIntersectionStatus;
    CarDirection endDirection;
    VehicleRecord vehicleRecord;
} Car;

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