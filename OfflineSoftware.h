/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This is the header file for the OfflineSoftware.c file. It contains the 
necessary structures and enumerations to be used in the OfflineSoftware.c file.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
} location;

typedef enum QUAD { NE, N, NW, E, W, SE, S, SW, NONE } QUAD;

typedef struct {
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    QUAD entrance;
} Customer;

typedef struct {
    int packageNum;
    int time;
    char event;
    int originCustomerID;
    int destinationCustomerID;
    float packageWeight;
    int deliveryTime;
    long prevSameSenderPos; // Pointer to the previous record with the same sender
} DeliveryRecord;
