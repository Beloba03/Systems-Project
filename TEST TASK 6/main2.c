#include "BldgGen.h"
#include <windows.h>

char** cityGrid;
FILE* bfd;
unsigned int xbldg, ybldg;

typedef struct {
    unsigned int x;
    unsigned int y;
} Car;

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

Car car;

void hideCursor() {
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO cursorInfo;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
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

    car.x = *xStart * 2;
    car.y = *yStart * 2;
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
            return ' ';
    }
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

void updateCar(CarDirection carDirection) {
    COORD prevPos = (COORD){car.x, car.y};
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
    setCursorPosition(prevPos.X, prevPos.Y);
    printf(" ");
    setCursorPosition(car.x, car.y);
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

void debugPrint(char* specState) {
    COORD tempCord = getCursorPosition();
    setCursorPosition(0, 2 * ybldg + 2);
    printf("Car X: %d, Car Y: %d, specState: %s        \n", car.x, car.y, specState);
    setCursorPosition(tempCord.X, tempCord.Y);
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

void animateCar(int xDestination, int yDestination) {
    int xDiff = xDestination * 2 - car.x;
    int yDiff = yDestination * 2 - car.y;

    while (xDiff != 0 || yDiff != 0) {
        // Calculate the horizontal and vertical move directions
        int xDirection = (xDiff > 0) ? 1 : (xDiff < 0) ? -1 : 0;
        int yDirection = (yDiff > 0) ? 1 : (yDiff < 0) ? -1 : 0;

        // Try to move horizontally if there is space
        if (xDiff != 0 && cityGrid[car.y][car.x + xDirection] == ' ') {
            updateCar((xDirection > 0) ? MOVE_RIGHT : MOVE_LEFT);
            xDiff -= xDirection;
            Sleep(200); // Add a delay for visualization using Sleep
        }
        // If horizontal move is not possible, try to move vertically
        else if (yDiff != 0 && cityGrid[car.y + yDirection][car.x] == ' ') {
            updateCar((yDirection > 0) ? MOVE_DOWN : MOVE_UP);
            yDiff -= yDirection;
            Sleep(200); // Add a delay for visualization using Sleep
        } else {
            // If no valid moves are possible, wait for 200 milliseconds
            Sleep(200);
        }
    }
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

    int xStart, yStart, xEnd, yEnd;
    getStartAndEndCoordinates(&xStart, &yStart, &xEnd, &yEnd);

    int destStatus = 0;
    while (destStatus == 0) {
        animateCar(xEnd, yEnd);
        destStatus = 1; // Set to 1 to exit the loop immediately after one animation
    }

    setCursorPosition(0, 2 * ybldg + 2);
    printf("                      ");
    setCursorPosition(0, 2 * ybldg + 2);
    printf("\nDone\n");

    printf("\nPress Enter to exit...");
    getchar();
}
