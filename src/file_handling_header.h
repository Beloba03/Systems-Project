#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directional_enums.h"
#include "car_header.h"
#define MAX_LINE_LENGTH 1024
#define BUILDING_OFFSET 3


typedef struct Customer{
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    enum QUAD entrance;
} Customer;

// External Variables
extern Car *car;
extern int numCars;
extern int tickTime;
extern void calcIntersection(int x, int y, int carNum);

// External Functions