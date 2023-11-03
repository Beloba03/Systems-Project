// Include the building generator header
#include "BldgGen.h"

// Main function
int main(int argc, char *argv[]) {
    // Check if the user has provided the correct number of arguments
    if (argc != 2) {
        printf("Supply name of data file\n");
        (void) getchar();  // Wait for a character input before exiting
        return 1;  // Return an error code
    }

    // Try to open the provided file for reading
    if ((bfd = fopen(argv[1], "r")) == NULL) {
        printf("Can't open %s\n", argv[1]);
        getchar();  // Wait for a character input before exiting
        return 1;  // Return an error code
    }
    // Read the file and set up the city grid layout
    read_file();
    hideCursor();
    setConsoleBufferSizeAndWindow(300, 300, 80, 50); // 80 columns wide, 200 rows buffer, 50 rows visible in window
    // Animate the car's movement on the console
    int destStatus = 0, wasDKeyPressed = 0;
    while (1) {
    int isDKeyPressed = GetAsyncKeyState('D') & 0x8000;

    if (isDKeyPressed && !wasDKeyPressed) {  // Key just got pressed
        updateEndCoordinates();  // Prompt user for new destination
    }

    wasDKeyPressed = isDKeyPressed;  // Update the state for the next iteration
    for(int i = 0; i < numCars; i++)
    {
        animateCar(i);
    }
    
    Sleep(50);  
}
    // Set the cursor position to the bottom of the grid
    setCursorPosition(0, 4*ybldg+7);
    printf("\nDone\n");  // Print the "Done" message
    (void) getchar();  // Wait for a character input before exiting
    freeGrid(ybldg);
    return 0;  // Return a success code
}
