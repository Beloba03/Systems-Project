/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This module contains functions relating to the car delivery queue linked list
*/

#include "linked_list_header.h"
#include "BldgGen.h"
void enqueue(int carNum, location loc) {
    // Create a new node
    LinkedList *newNode = (LinkedList *)malloc(sizeof(LinkedList));
    if(newNode == NULL)
    {
        printf("Error allocating memory for new node\n");
        exit(1);
    }
    newNode->loc = loc;
    newNode->next = NULL;

    // Traverse to the end of the queue and add the new node
    LinkedList *current = &car[carNum].locQueue;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}


location dequeue(int carNum) {
    // Check if the queue is empty
    if (car[carNum].locQueue.next == NULL) {
        // Handle the empty queue case
        location errorLoc;
        errorLoc.endPos.X = 0;
        errorLoc.endPos.Y = 0;
        errorLoc.endDir = 0;
        return errorLoc;
    }

    // Get the first node
    LinkedList *firstNode = car[carNum].locQueue.next;

    // Update the head to point to the next node
    car[carNum].locQueue.next = firstNode->next;

    // Store the location data to return
    location loc = firstNode->loc;
    //car[carNum].delTime = loc.time;

    // Free the dequeued node
    free(firstNode);

    return loc; // Return the location from the removed node
}




