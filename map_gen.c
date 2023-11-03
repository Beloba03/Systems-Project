#include "BldgGen.h"

// Declaration of a 2D character array for city grid layout
char** cityGrid;
COORD startOffset;
unsigned int xbldg, ybldg, s1dir, a1dir;
struct bldg_data bd;  // Declare a building data structure

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
    cityGrid = (char**)malloc((4 * ySize + 6) * sizeof(char*));
    if(cityGrid == NULL)
    {
        printf("Failed to allocate memory for cityGrid");
        getchar();
        exit(EXIT_FAILURE);
    }
    int skipVer = 1, skipHor;
    for (unsigned int i = 0; i < 4 * ySize + 2; i++) {
        skipHor = 1;
        cityGrid[i] = (char*)malloc((4 * xSize + 2) * sizeof(char));
        if(cityGrid[i] == NULL)
        {
            printf("Failed to allocate memory for cityGrid[%d]", i);
            getchar();
            exit(EXIT_FAILURE);
        }
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

void printGrid(unsigned int xSize, unsigned int ySize) {
    setCursorPosition(startOffset.X, startOffset.Y);
    for (unsigned int y = 0; y < (ySize * 4) + 1; y++) {
        for (unsigned int x = 0; x < (xSize * 4) + 1; x++) {
                printf("%c", cityGrid[y][x]);
        }
        printf("\n");
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

// Function to read building data from a file and set up the city grid layout
void read_file() {

    // Read x and y dimensions of the building from the file
    fread(&xbldg, sizeof(int), 1, bfd);
    fread(&ybldg, sizeof(int), 1, bfd);

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
        if (bd.bt == CHG) {  // Using strcmp since we are comparing strings
            tempCoord = convertBuildingCoord(bd.x*4-2, bd.y*4-2, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'C';
        } else if (bd.bt == STB) {
            tempCoord = convertBuildingCoord(bd.x*4-2, bd.y*4-2, bd.qd);
            cityGrid[tempCoord.Y][tempCoord.X] = 'S';
        } else if (bd.bt == BOTH) {
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

    
}

// Function to free allocated memory for the city grid
void freeGrid(unsigned int ySize) {
    for (unsigned int i = 0; i < (4 * ySize + 2); i++) {
        free(cityGrid[i]);
    }
    free(cityGrid);
    free(car);
}