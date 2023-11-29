#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "directional_enums.h"
#define MAX_LINE_LENGTH 256
#define BUILDING_OFFSET 3


// Structure to define a delivery request
typedef struct DeliveryRequest{
    int time;
    char event;
    char origin_customer_id[50];
    char destination_customer_id[50];
    float package_weight;
} DeliveryRequest;

typedef struct Customer{
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    enum QUAD entrance;
} Customer;

// External Variables
extern int time;
extern Car *car;

// External Functions