 #include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h> // Include for _beginthread

#include "BldgGen.h"

FILE* bfd;

char** cityGrid;
unsigned int xbldg, ybldg;

typedef struct {
    unsigned int x;
    unsigned int y;
} CarPosition;

typedef struct {
    CarPosition currentPos;
    CarPosition startPos;
    CarPosition endPos;
} Car;

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

Car* cars; // Array to store multiple cars
int numCars; // Number of cars in the simulation

void hideCursor() {
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO cursorInfo;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

void getStartAndEndCoordinates(Car* car) {
    printf("Enter the X-coordinate for the starting point: ");
    scanf("%d", &(car->startPos.x));
    while (getchar() != '\n'); // Clear the input buffer

    printf("Enter the Y-coordinate for the starting point: ");
    scanf("%d", &(car->startPos.y));
    while (getchar() != '\n'); // Clear the input buffer

    printf("Enter the X-coordinate for the ending point: ");
    scanf("%d", &(car->endPos.x));
    while (getchar() != '\n'); // Clear the input buffer

    printf("Enter the Y-coordinate for the ending point: ");
    scanf("%d", &(car->endPos.y));
    while (getchar() != '\n'); // Clear the input buffer

    // Initialize the car's current position to its starting position
    car->currentPos = car->startPos;
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD getCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return csbi.dwCursorPosition;
    } else {
        COORD defaultCoord = {0, 0};
        return defaultCoord;
    }
}

void updateCar(Car* car, CarDirection carDirection) {
    COORD prevPos = (COORD){car->currentPos.x, car->currentPos.y};
    switch (carDirection) {
        case MOVE_UP:
            car->currentPos.y--;
            break;
        case MOVE_RIGHT:
            car->currentPos.x++;
            break;
        case MOVE_DOWN:
            car->currentPos.y++;
            break;
        case MOVE_LEFT:
            car->currentPos.x--;
    }
    setCursorPosition(prevPos.X, prevPos.Y);
    printf(" ");
    setCursorPosition(car->currentPos.x, car->currentPos.y);
    printf("*");
}

void initializeGrid(unsigned int xSize, unsigned int ySize) {
    cityGrid = (char**)malloc((2 * ySize + 3) * sizeof(char*)); // Increase the size by 1 for road edges
    for (unsigned int i = 0; i < (2 * ySize + 3); i++) { // Increase the size by 1 for road edges
        cityGrid[i] = (char*)malloc((2 * xSize + 3) * sizeof(char)); // Increase the size by 1 for road edges
        for (unsigned int j = 0; j < (2 * xSize + 3); j++) { // Increase the size by 1 for road edges
            if (i == 0 || i == (2 * ySize + 2) || j == 0 || j == (2 * xSize + 2)) {
                // Create road edges at the boundaries
                cityGrid[i][j] = ' ';
            } else if (i % 2 == 0 || j % 2 == 0) {
                cityGrid[i][j] = ' ';
            } else {
                cityGrid[i][j] = '#';
            }
        }
    }
}

int isPathClear(int startX, int startY, int endX, int endY) {
    // Check if there are any buildings in the path
    int xDiff = endX - startX;
    int yDiff = endY - startY;

    if (xDiff > 0) {
        for (int x = startX + 1; x < endX; x++) {
            if (cityGrid[startY][x] != ' ') {
                return 0; // Path is not clear
            }
        }
    } else if (xDiff < 0) {
        for (int x = startX - 1; x > endX; x--) {
            if (cityGrid[startY][x] != ' ') {
                return 0; // Path is not clear
            }
        }
    } else if (yDiff > 0) {
        for (int y = startY + 1; y < endY; y++) {
            if (cityGrid[y][startX] != ' ') {
                return 0; // Path is not clear
            }
        }
    } else if (yDiff < 0) {
        for (int y = startY - 1; y > endY; y--) {
            if (cityGrid[y][startX] != ' ') {
                return 0; // Path is not clear
            }
        }
    }

    return 1; // Path is clear
}

// Function to animate a car
int animateCar(Car* car) {
    int xDiff = (car->endPos.x - car->currentPos.x) * 2; // Adjust for larger grid cells
    int yDiff = (car->endPos.y - car->currentPos.y) * 2; // Adjust for larger grid cells

    while (xDiff != 0 || yDiff != 0) {
        // Calculate the horizontal and vertical move directions
        int xDirection = (xDiff > 0) ? 1 : (xDiff < 0) ? -1 : 0;
        int yDirection = (yDiff > 0) ? 1 : (yDiff < 0) ? -1 : 0;

        // Try to move horizontally if there is space
        if (xDiff != 0 && cityGrid[car->currentPos.y][car->currentPos.x + xDirection] == ' ') {
            updateCar(car, (xDirection > 0) ? MOVE_RIGHT : MOVE_LEFT);
            xDiff -= xDirection;
            Sleep(200); // Add a delay for visualization using Sleep
        }
        // If horizontal move is not possible, try to move vertically
        else if (yDiff != 0 && cityGrid[car->currentPos.y + yDirection][car->currentPos.x] == ' ') {
            updateCar(car, (yDirection > 0) ? MOVE_DOWN : MOVE_UP);
            yDiff -= yDirection;
            Sleep(200); // Add a delay for visualization using Sleep
        } else {
            // If no valid moves are possible, wait for 200 milliseconds
            Sleep(200);
        }
    }

    // Return 1 only after reaching the destination
    return 1; // Successful animation
}

// Thread function to animate a car
void animateCarThread(void* param) {
    Car* car = (Car*)param;
    animateCar(car);
}

void freeGrid(unsigned int ySize) {
    for (unsigned int i = 0; i < ySize + 1; i++) {
        free(cityGrid[i]);
    }
    free(cityGrid);
}

void printGrid(unsigned int xSize, unsigned int ySize) {
    for (unsigned int y = 0; y < (ySize * 2) + 1; y++) {
        for (unsigned int x = 0; x < (xSize * 2) + 1; x++) {
            printf("%c", cityGrid[y][x]);
        }
        printf("\n");
    }
}

void read_file() {
    struct bldg_data bd;

    fread(&xbldg, sizeof(int), 1, bfd);
    fread(&ybldg, sizeof(int), 1, bfd);

    initializeGrid(xbldg, ybldg);

    fread(&bd, sizeof(struct bldg_data), 1, bfd);
    while (bd.x > 0) {
        if (bd.bt == CHG) {
            cityGrid[bd.y][bd.x] = 'C';
        } else if (bd.bt == STB) {
            cityGrid[bd.y][bd.x] = 'S';
        } else if (bd.bt == BOTH) {
            cityGrid[bd.y][bd.x] = 'B';
        }
        fread(&bd, sizeof(struct bldg_data), 1, bfd);
    }

    fclose(bfd);
    printGrid(xbldg, ybldg);
    freeGrid(ybldg);
}

void initializeCars(int num) {
    numCars = num;
    cars = (Car*)malloc(numCars * sizeof(Car));
    if (cars == NULL) {
        printf("Memory allocation failed for cars.\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Supply the name of the data file\n");
        (void)getchar();
        return 1;
    }

    if ((bfd = fopen(argv[1], "r")) == NULL) {
        printf("Can't open %s\n", argv[1]);
        (void)getchar();
        return 1;
    }

    hideCursor();

    read_file();

    printf("Enter the number of cars: ");
    scanf("%d", &numCars);

    // Clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    initializeCars(numCars);

    HANDLE* carThreads = (HANDLE*)malloc(numCars * sizeof(HANDLE));
    if (carThreads == NULL) {
        printf("Memory allocation failed for carThreads.\n");
        exit(1);
    }

    // Start a separate thread for each car
    for (int i = 0; i < numCars; i++) {
        getStartAndEndCoordinates(&cars[i]);
        carThreads[i] = (HANDLE)_beginthread(animateCarThread, 0, &cars[i]);
    }

    int destStatus = numCars; // Initialize destStatus to the number of cars

    // Update the console while animation is in progress
    while (destStatus > 0) {
        // Sleep briefly to avoid high CPU usage
        Sleep(100);

        // Check if any cars have reached their destinations
        destStatus = 0;
        for (int i = 0; i < numCars; i++) {
            if (cars[i].currentPos.x != cars[i].endPos.x || cars[i].currentPos.y != cars[i].endPos.y) {
                destStatus++;
            }
        }
    }

    // Close thread handles
    for (int i = 0; i < numCars; i++) {
        CloseHandle(carThreads[i]);
    }

    free(carThreads); // Free thread handles
    free(cars); // Free memory for cars

    return 0;
}