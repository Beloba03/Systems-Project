#include "linked_list_header.h"

void enqueue(int carNum, location loc) {
    // Create a new node
    LinkedList *newNode = (LinkedList *)malloc(sizeof(LinkedList));
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
        // Handle the empty queue case, perhaps return a special location or an error
        location errorLoc;
        errorLoc.endPos.X = 0;
        errorLoc.endPos.Y = 0;
        errorLoc.endDir = 0; // Assuming 0 is an invalid direction or a placeholder
        return errorLoc;
    }

    // Get the first node (after the dummy head)
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



