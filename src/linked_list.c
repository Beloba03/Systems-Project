#include "linked_list_header.h"
#include "BldgGen.h"
void enqueue(int carNum, location loc) {
    static int runOnce = 0;
    if(runOnce == 0)
    {
        for(int i = 0; i < numCars; i++)
        {
            car[i].locQueue.next = NULL;
            car[i].currentLoc = &car[i].locQueue;
        }
        runOnce = 1;
    }
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

    // Free the dequeued node
    free(firstNode);

    return loc; // Return the location from the removed node
}

location peek(int carNum, int locationPos) {
    LinkedList *current = car[carNum].locQueue.next;

    // Traverse to the specified position
    for (int i = 0; i < locationPos && current != NULL; i++) {
        current = current->next;
    }

    // Check if the specified position is beyond the end of the queue
    if (current == NULL) {
        // Handle the case where the position is out of bounds
        location errorLoc;
        errorLoc.endPos.X = 0;
        errorLoc.endPos.Y = 0;
        errorLoc.endDir = 0; // Assuming 0 is an invalid direction or a placeholder
        return errorLoc;
    }

    // Return the location at the specified position
    return current->loc;
}
void printQueue(int carNum) {
    COORD oldPos = getCursorPosition();
    setCursorPosition(0, 80);
    // Start from the first node (skip the dummy head)
    LinkedList *current = car[carNum].locQueue.next;

    // Check if the queue is empty
    if (current == NULL) {
        car[carNum].endIntersectionStatus = 8;
        printf("Car %d's queue is empty.\n", carNum);
        return;
    }

    printf("Queue for Car %d:\n", carNum);
    while (current != NULL) {
        // Print each location's details
        printf("Location - X: %d, Y: %d, Direction: %d, Floor: %d\n",
               current->loc.endPos.X, current->loc.endPos.Y, 
               current->loc.endDir, current->loc.floorNum);

        // Move to the next node
        current = current->next;
    }
    for(int i = 0; i < 10; i++)
        printf("                                                                                           \n");
    setCursorPosition(oldPos.X, oldPos.Y);
}



