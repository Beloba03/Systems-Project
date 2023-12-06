#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directional_enums.h"
#include "car_header.h"
#define MAX_LINE_LENGTH 1024
#define START_PACKAGE_NUM 500
#define HEADER_SIZE sizeof(DeliveryRecord)


typedef struct Customer{
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    enum QUAD entrance;
} Customer;

typedef struct {
    int packageNum;
    int time;
    char event;
    int originCustomerID;
    int destinationCustomerID;
    float packageWeight;
    int deliveryTime;
    long prevSameSenderPos; // Field to store the position of the previous record with the same sender
} DeliveryRecord;

char *delimiter = ",\t\n ";

// External Variables
extern Car *car;
extern EventRecord *currentEvents;
extern int numCars;
extern int tickTime;

// External Functions
extern void calcIntersection(int x, int y, int carNum);
