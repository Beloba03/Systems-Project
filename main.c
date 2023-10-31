// Include the building generator header
#include "BldgGen.h"
// Include the Windows API header (to handle Windows-specific functionalities)
#include <windows.h>

// Declaration of a 2D character array for city grid layout
char** cityGrid;

// Variables to store x and y dimensions of a building
unsigned int xbldg, ybldg, s1dir, a1dir;

// Define a structure named Car to hold the x and y position of a car in the city grid
typedef struct {
    unsigned int x;  // x-position of the car
    unsigned int y;  // y-position of the car
} Car;


typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

// Declare a variable of type Car
Car car;

void hideCursor() {
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO cursorInfo;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    cursorInfo.bVisible = FALSE; // Set the cursor visibility to FALSE

    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

char getBuildingTypeRepresentation(enum BLDG_TYPE type) {
    switch (type) {
        case CHG:
            return 'C';
        case STB:
            return 'S';
        case BOTH:
            return 'B';
        default:
            return ' '; // Default case to handle unexpected values
    }
}

COORD convertBuildingCoord(int x, int y, enum QUAD location)
{
    COORD tempCord = {x, y};
    switch(location) {
        case NE:
            tempCord.X -= 1;
            tempCord.Y -= 1;
            break;
        case N:
            tempCord.Y -= 1;
            break;
        case NW:
            tempCord.X += 1;
            tempCord.Y -= 1;
            break;
        case W:
            tempCord.X += 1;
            break;
        case SW:
            tempCord.X += 1;
            tempCord.Y += 1;
            break;
        case S:
            tempCord.Y += 1;
            break;
        case SE:
            tempCord.X -= 1;
            tempCord.Y += 1;
            break;
        case E:
            tempCord.X -= 1;
            break;
        default:
            break;
    }
    return tempCord;
}
void getStartAndEndCoordinates(int* xStart, int* yStart, int* xEnd, int* yEnd) {
    printf("Enter the X-coordinate for the starting point: ");
    scanf("%d", xStart);

    printf("Enter the Y-coordinate for the starting point: ");
    scanf("%d", yStart);

    printf("Enter the X-coordinate for the ending point: ");
    scanf("%d", xEnd);

    printf("Enter the Y-coordinate for the ending point: ");
    scanf("%d", yEnd);

    getchar(); // Clear any remaining newline characters from the input buffer

    car.x = *xStart * 4;
    car.y = *yStart * 4;
}

// Function to set the console's cursor position
void setCursorPosition(int x, int y) {
    COORD coord;  // Use the COORD struct from Windows API
    coord.X = x;  // Set the X coordinate
    coord.Y = y;  // Set the Y coordinate
    // Set the cursor position using the Windows API function
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD getCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition;
    } else {
        // If there's an error, return a default value or handle as appropriate
        COORD defaultCoord = {0, 0};
        return defaultCoord;
    }
}

// Function to update car's position on the console
void updateCar(CarDirection carDirection) {
    COORD prevPos = (COORD){car.x, car.y};  // Store the previous position (before updating
    // Update car's position based on direction
    switch (carDirection) {
        case MOVE_UP:
                car.y--;
            break;
        case MOVE_RIGHT:
                car.x++;
            break;
        case MOVE_DOWN:
                car.y++;
            break;
        case MOVE_LEFT:
                car.x--;
    }
    setCursorPosition(prevPos.X, prevPos.Y);  // Move cursor to previous position
    printf(" "); // Clear the previous position
    setCursorPosition(car.x, car.y);  // Update for new position
    printf("*");
}


// Function to initialize the city grid
void initializeGrid(unsigned int xSize, unsigned int ySize) {
    cityGrid = (char**)malloc((4 * ySize + 2) * sizeof(char*));
    int skipVer = 1, skipHor;
    for (unsigned int i = 0; i < 4 * ySize + 2; i++) {
        skipHor = 1;
        cityGrid[i] = (char*)malloc((4 * xSize + 2) * sizeof(char));
        for (unsigned int j = 0; j < 4 * xSize + 2; j++) {
            if (i % 4 == 0 || j % 4 == 0) {
                cityGrid[i][j] = ' ';
                skipHor = !skipHor;
            }
            else if(skipVer != 0){
                if(skipHor != 1)
                {
                    cityGrid[i][j] = 'O';
                    skipHor = !skipHor;
                }
                else
                {
                    cityGrid[i][j] = '#';
                    skipHor = !skipHor;
                }
            }
            else
                cityGrid[i][j] = 'O';
        }
        skipVer = !skipVer;
    }
}
void debugPrint(char *specState)
{
    COORD tempCord = getCursorPosition();
    setCursorPosition(0, 4*ybldg+8);
    printf("Car X: %d, Car Y: %d, specState: %s        \n", car.x, car.y, specState);
    setCursorPosition(tempCord.X, tempCord.Y);
}
// Function to animate car movement on the console
int animateCar(int xDestination, int yDestination) {
    // Condition to stop the loop when the car completes a full perimeter loop
    unsigned int startX = car.x;
    unsigned int startY = car.y;
    COORD tempCord;

    if(car.x < xDestination * 4) {
        updateCar(MOVE_RIGHT);
        debugPrint("XR");
    }   
    else if(car.x > xDestination * 4) {
        updateCar(MOVE_LEFT);
        debugPrint("XL");
    }
    else if(car.y < yDestination * 4+4) {
        updateCar(MOVE_DOWN);
        debugPrint("YD");
    }
    else if(car.y > yDestination * 4+4) {
        updateCar(MOVE_UP);
        debugPrint("YU");
    }
    else {
        return 1;
    }
    Sleep(200);
    return 0;
}

// Function to free allocated memory for the city grid
void freeGrid(unsigned int ySize) {
    // Free memory for each row
    for (unsigned int i = 0; i < ySize + 1; i++) {
        free(cityGrid[i]);
    }
    // Free memory for the city grid
    free(cityGrid);
}

void printGrid(unsigned int xSize, unsigned int ySize) {
    for (unsigned int y = 0; y < (ySize * 4) + 1; y++) {
        for (unsigned int x = 0; x < (xSize * 4) + 1; x++) {
                printf("%c", cityGrid[y][x]);
        }
        printf("\n");
    }
}


// Function to read building data from a file and set up the city grid layout
void read_file() {
    struct bldg_data bd;  // Declare a building data structure

    // Read x and y dimensions of the building from the file
    fread(&xbldg, sizeof(int), 1, bfd);
    fread(&ybldg, sizeof(int), 1, bfd);

    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);

    // Initialize the city grid based on the read dimensions
    initializeGrid(xbldg, ybldg);

    // Read building data and set up the city grid
    fread(&bd, sizeof(struct bldg_data), 1, bfd);
    // Iterate until a building with x-coordinate greater than 0 is found
    COORD tempCoord;
    while (bd.x > 0) {
        // Depending on the building type, update the corresponding cell in the city grid
        if (strcmp(bldg_t[bd.bt].name, "Charge") == 0) {  // Using strcmp since we are comparing strings
            tempCoord = convertBuildingCoord(bd.x*4-2, bd.y*4-2, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'C';
        } else if (strcmp(bldg_t[bd.bt].name, "Stable") == 0) {
            tempCoord = convertBuildingCoord(bd.x*4-2, bd.y*4-2, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'S';
        } else if (strcmp(bldg_t[bd.bt].name, "Both") == 0) {
            tempCoord = convertBuildingCoord(bd.x*4-2, bd.y*4-2, bd.qd);
            // printf("X: %d(%d), Y: %d(%d), QUAD: %s\n", bd.x, bd.x*4-2, bd.y, bd.y*4-2,bldg_q[bd.qd].name);
            // printf("tX: %d, tY: %d\n", tempCoord.X, tempCoord.Y);
            cityGrid[tempCoord.Y][tempCoord.X] = 'B';
        }
        
        // Print building details on the console
        //printf("Bldg XY: %d %d Type: %s Quad: %s\n", bd.x, bd.y, 
         //   bldg_t[bd.bt].name, bldg_q[bd.qd].name);

        // Read the next building data
        fread(&bd, sizeof(struct bldg_data), 1, bfd);
    }

    // Close the file
    fclose(bfd);
    // Print the city grid on the console
    printGrid(xbldg, ybldg);
    // Free the allocated memory for the city grid
    freeGrid(ybldg);
}


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
    int xStart, yStart, xEnd, yEnd;
    getStartAndEndCoordinates(&xStart, &yStart, &xEnd, &yEnd);
    hideCursor();
    // Read the file and set up the city grid layout
    read_file();
    // Animate the car's movement on the console
    int destStatus = 0;
    while(destStatus == 0)
    {
        destStatus = animateCar(xEnd, yEnd);
    }
    
    // Set the cursor position to the bottom of the grid
    setCursorPosition(0, 4*ybldg+7);
    printf("\nDone\n");  // Print the "Done" message
    (void) getchar();  // Wait for a character input before exiting

    return 0;  // Return a success code
}
