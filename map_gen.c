/*
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This module contains functions relating to reading and creating the map
*/

#include "BldgGen.h"

// Declaration of a 2D character array for city grid layout
char** cityGrid;
COORD startOffset;
unsigned int xbldg, ybldg, s1dir, a1dir;
int numCars;
struct bldg_data bd;  // Declare a building data structure

// This converts the quadrant to a string
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

// Function to initialize the city grid
void initializeGrid(unsigned int xSize, unsigned int ySize) {
    cityGrid = (char**)malloc((SCALE_FACTOR * ySize + 6) * sizeof(char*)); // Allocate memory for the city grid (y)
    if(cityGrid == NULL) // Check if memory allocation was successful
    {
        printf("Failed to allocate memory for cityGrid");
        getchar();
        exit(EXIT_FAILURE);
    }
    int skipVer = 1, skipHor; // Variables to track which cells to skip (the streets)

    // Iterate through the city grid and set the appropriate cells to the appropriate characters
    for (unsigned int i = 0; i < SCALE_FACTOR * ySize + 2; i++) {
        skipHor = 1;
        cityGrid[i] = (char*)malloc((SCALE_FACTOR * xSize + 2) * sizeof(char)); // Allocate memory for the city grid (x)
        if(cityGrid[i] == NULL)
        {
            printf("Failed to allocate memory for cityGrid[%d]", i);
            getchar();
            exit(EXIT_FAILURE);
        }
        // Iterate through each line of the city grid
        for (unsigned int j = 0; j < SCALE_FACTOR * xSize + 2; j++) {
            if (i % SCALE_FACTOR == 0 || j % SCALE_FACTOR == 0 || i % SCALE_FACTOR == 1 || j % SCALE_FACTOR == 1) {
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

// Prints the city grid to the console
void printGrid(unsigned int xSize, unsigned int ySize) {
    setCursorPosition(startOffset.X, startOffset.Y);
    for (unsigned int y = 0; y < (ySize * SCALE_FACTOR) + 1; y++) {
        for (unsigned int x = 0; x < (xSize * SCALE_FACTOR) + 1; x++) {
                printf("%c", cityGrid[y][x]);
        }
        printf("\n");
    }
}

// Function to convert from the centre of the building to each cardinal direction
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

// Function to read building data from a file and set up the city grid layout
void read_file() {

    // Read x and y dimensions of the building from the file
    fread(&xbldg, sizeof(int), 1, bfd);
    fread(&ybldg, sizeof(int), 1, bfd);
    numCars = getStartAndEndCoordinates();
    // 0 = East, 1 = West
    // 0 = North, 1 = South
    fread(&s1dir, sizeof(int), 1, bfd);
    fread(&a1dir, sizeof(int), 1, bfd);

    // Initialize the city grid based on the read dimensions
    initializeGrid(xbldg, ybldg);

    // Read building data and set up the city grid
    fread(&bd, sizeof(struct bldg_data), 1, bfd);
    // Iterate while a building with x-coordinate greater than 0 is found
    COORD tempCoord;
    while (bd.x > 0) {
        // Depending on the building type, update the corresponding cell in the city grid
        if (bd.bt == CHG) {
            tempCoord = convertBuildingCoord(bd.x*SCALE_FACTOR-BUILDING_OFFSET, bd.y*SCALE_FACTOR-BUILDING_OFFSET, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'C';
        } else if (bd.bt == STB) {
            tempCoord = convertBuildingCoord(bd.x*SCALE_FACTOR-BUILDING_OFFSET, bd.y*SCALE_FACTOR-BUILDING_OFFSET, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'S';
        } else if (bd.bt == BOTH) {
            tempCoord = convertBuildingCoord(bd.x*SCALE_FACTOR-BUILDING_OFFSET, bd.y*SCALE_FACTOR-BUILDING_OFFSET, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'B';
        }

        // Read the next building data
        fread(&bd, sizeof(struct bldg_data), 1, bfd);
    }
    // Close the file
    fclose(bfd);
    // Print the city grid on the console
    printGrid(xbldg, ybldg);

    
}

// Function to free allocated memory for the city grid and cars
void freeGrid(unsigned int ySize) {
    for (unsigned int i = 0; i < (SCALE_FACTOR * ySize + 2); i++) {
        free(cityGrid[i]);
    }
    free(cityGrid);
    free(car);
}