/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This is the main module for the AEDV program. It contains the main function which controls
and calls all of the other functions
*/

// Include the building generator header.
#include "BldgGen.h"
#include "linked_list_header.h"

int tickTime = 0; // Main time variable
int numCars = 0; // Number of cars

// Main function to demonstrate the building generator.
int main(int argc, char *argv[]) {
    
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
    read_file(); // Read the data from the file and set up the city grid layout.
    convCustToRel(); // Convert the customer file to a realative file
    sortVehicles(); // Sort the vehicle file and ask user what vehicles they want to use
    sortEvents(); // Sort the event file be time


    while (GetAsyncKeyState(VK_ESCAPE) >= 0) {  // Run program until user presses 'esc'.
        printTime(tickTime);    // Print the current time.

        // Animate each car on the grid.
        for (int i = 0; i < numCars; i++) {
            animateCarNew(i);
        }
        tickTime++; // Increment the time.
        Sleep(20); // Sleep for 20 milliseconds so cars don't teleport.
    }

    setCursorPosition(0, SCALE_FACTOR*ybldg); // Set the cursor position to the bottom of the grid.
    printf("\nDone\n");  // Inform the user that the animation is complete.

    (void) getchar();    // Wait for a character input before exiting.
    getchar();

    // Clean up resources.
    freeGrid(ybldg);
    getCurrentEvent(1);

    return 0; // Return a success code.
}
