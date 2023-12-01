/*
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This is the main module for the AEDV program. It contains the main function which controls
and calls all of the other functions
*/

// Include the building generator header.
#include "BldgGen.h"
#include "linked_list_header.h"
int tickTime = 0;
// Main function to demonstrate the building generator.
int main(int argc, char *argv[]) {

    tickTime = 0; // CHANGE TO GET TIME FROM SAVE FILE
    
    // Check if the user has provided the correct number of arguments.
    if (argc != 2) {
        argv[1] = "runFiles\\Test";
    }

    // Attempt to open the provided data file for reading.
    bfd = fopen(argv[1], "r");
    if (bfd == NULL) {
        printf("Can't open %s\n", argv[1]);
        getchar();      // Wait for a character input before exiting.
        return 1;       // Return an error code.
    }

     // Read the data from the file and set up the city grid layout.
    read_file();
    // Prepare the console for the animation.
    setConsoleBufferSizeAndWindow(1000, 600, 80, 80); // Set buffer and window sizes.
    hideCursor();
    staticCarNum();
    sortEvents();
    convCustToRel();
    setCarDest(0);

    // First request
    printQueue(0);
    location req1 = dequeue(0);
    car[0].x = req1.endPos.X;
    car[0].y = req1.endPos.Y;
    location req2 = dequeue(0);
    car[0].endPos.X = req2.endPos.X;
    car[0].endPos.Y = req2.endPos.Y;
    car[0].endDirection = req2.endDir;
    calcIntersection(req2.endPos.X, req2.endPos.Y, 0);



    //preventAutoScroll(); // HASN'T BEEN IMPLEMENTED YET


    for(int i = 0; i < numCars; i++)
    {
        car[i].locQueue.next = NULL;
        car[i].currentLoc = &car[i].locQueue;
    }
   

    

    // Variables to track the car's movement on the console.
    int destStatus = 0, wasDKeyPressed = 0;
    
    while (GetAsyncKeyState(VK_ESCAPE) >= 0) {  // Run program until user presses 'esc'.

        // Animate each car on the grid.
        for (int i = 0; i < numCars; i++) {
            int endIX = car[i].endIntersection.X, endIY = car[i].endIntersection.Y, endX = car[i].endPos.X, endY = car[i].endPos.Y, curx = car[i].x, cury = car[i].y;
            animateCarNew(i);
        }
        
        tickTime++;
        Sleep(200);
    }
    // Set the cursor position to the bottom of the grid.
    setCursorPosition(0, SCALE_FACTOR*ybldg+7);
    printf("\nDone\n");  // Inform the user that the animation is complete.

    (void) getchar();    // Wait for a character input before exiting.

    // Clean up resources.
    freeGrid(ybldg);

    return 0; // Return a success code.
}
