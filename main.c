/*
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This is the main module for the AEDV program. It contains the main function which controls
and calls all of the other functions
*/

// Include the building generator header.
#include "BldgGen.h"

// Main function to demonstrate the building generator.
int main(int argc, char *argv[]) {
    
    // Check if the user has provided the correct number of arguments.
    if (argc != 2) {
        printf("Supply name of data file\n");
        (void) getchar(); // Wait for a character input before exiting.
        return 1;        // Return an error code.
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
    hideCursor();
    setConsoleBufferSizeAndWindow(600, 600, 80, 80); // Set buffer and window sizes.

    // Variables to track the car's movement on the console.
    int destStatus = 0, wasDKeyPressed = 0;
    while (GetAsyncKeyState(VK_ESCAPE) >= 0) {  // Run program until user presses 'esc'.
        // Check if the 'D' key is pressed.
        int isDKeyPressed = GetAsyncKeyState('D') & 0x8000;

        // If the 'D' key was just pressed, update the car's destination coordinates.
        if (isDKeyPressed && !wasDKeyPressed) {
            updateEndCoordinates();
        }

        // Update the state of the 'D' key for the next iteration.
        wasDKeyPressed = isDKeyPressed;

        // Animate each car on the grid.
        for (int i = 0; i < numCars; i++) {
            animateCarNew(i);
        }
    }
    // Set the cursor position to the bottom of the grid.
    setCursorPosition(0, SCALE_FACTOR*ybldg+7);
    printf("\nDone\n");  // Inform the user that the animation is complete.

    (void) getchar();    // Wait for a character input before exiting.

    // Clean up resources.
    freeGrid(ybldg);

    return 0; // Return a success code.
}

//AARON WAS HERE, testing out commit
