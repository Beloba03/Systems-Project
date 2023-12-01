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
int numCars = 0;
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
    setConsoleBufferSizeAndWindow(1000, 600, 80, 80); // Set buffer and window sizes.
     // Read the data from the file and set up the city grid layout.
    read_file();
    sortVehicles();
    initCars();
    // Prepare the console for the animation.
    staticCarNum();
    sortEvents();
    convCustToRel();
    setCarDest();
    // Variables to track the car's movement on the console.
    int destStatus = 0, wasDKeyPressed = 0;
    while (GetAsyncKeyState(VK_ESCAPE) >= 0) {  // Run program until user presses 'esc'.

        // Animate each car on the grid.
        for (int i = 0; i < numCars; i++) {
            int endIX = car[i].endIntersection.X, endIY = car[i].endIntersection.Y, endX = car[i].endPos.X, endY = car[i].endPos.Y, curx = car[i].x, cury = car[i].y;
            animateCarNew(i);
        }
        
        tickTime++;
        Sleep(20);
    }
    // Set the cursor position to the bottom of the grid.
    setCursorPosition(0, SCALE_FACTOR*ybldg+7);
    printf("\nDone\n");  // Inform the user that the animation is complete.

    (void) getchar();    // Wait for a character input before exiting.

    // Clean up resources.
    freeGrid(ybldg);

    return 0; // Return a success code.
}
