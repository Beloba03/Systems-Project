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
    COORD endPos;
    enum QUAD endQuad;
} Car;
COORD startOffset;

typedef enum {
    MOVE_UP,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT
} CarDirection;

// Declare a variable of type Car
Car *car;
struct bldg_data bd;  // Declare a building data structure

void hideCursor() {
    HANDLE hConsoleOutput;
    CONSOLE_CURSOR_INFO cursorInfo;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

    cursorInfo.bVisible = FALSE; // Set the cursor visibility to FALSE

    SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}
COORD convertBuildingCoordToStreet(COORD tempCoord, enum QUAD location)
{
    switch(location) {
        case NE:
            tempCoord.X -= 1;
            tempCoord.Y -= 2;
            break;
        case N:
            tempCoord.Y -= 2;
            break;
        case NW:
            tempCoord.X += 1;
            tempCoord.Y -= 2;
            break;
        case W:
            tempCoord.X += 2;
            break;
        case SW:
            tempCoord.X += 1;
            tempCoord.Y += 2;
            break;
        case S:
            tempCoord.Y += 2;
            break;
        case SE:
            tempCoord.X -= 1;
            tempCoord.Y += 2;
            break;
        case E:
            tempCoord.X -= 1;
            break;
        default:
            break;
    }
    return tempCoord;
}
void setConsoleBufferSizeAndWindow(short xBuffer, short yBuffer, short xWindow, short yWindow) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set buffer size
    COORD bufferSize = {xBuffer, yBuffer};
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set window size
    SMALL_RECT windowSize = {0, 0, xWindow - 1, yWindow - 1}; // -1 because coordinates are zero-based
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
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

// Function to set the console's cursor position
void setCursorPosition(int x, int y) {
    COORD coord;  // Use the COORD struct from Windows API
    coord.X = x;  // Set the X coordinate
    coord.Y = y;  // Set the Y coordinate
    // Set the cursor position using the Windows API function
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int isSpaceFree(int x, int y) {
    if (cityGrid[y-startOffset.Y][x] == 'C' || cityGrid[y-startOffset.Y][x] == 'S' || cityGrid[y-startOffset.Y][x] == 'B' || cityGrid[y-startOffset.Y][x] == '#' || cityGrid[y-startOffset.Y][x] == 'O') {
        return 0; // Space is not free (occupied by a building)
    }
    return 1; // Space is free
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
// Function to map integer to QUAD enum
enum QUAD mapIntToQuad(int quadInt) {
    switch(quadInt) {
        case 1:
            return N;
        case 2:
            return NW;
        case 3:
            return W;
        case 4:
            return SW;
        case 5:
            return S;
        case 6:
            return SE;
        case 7:
            return E;
        case 8:
            return NE;
        default:
            printf("Invalid quadrant number entered.\n");
            exit(1);
    }
}

// void debugPrint(int state, int other, int under, int complete) {
//     COORD tempCord = getCursorPosition();
//     setCursorPosition(0, 4*ybldg+8);
//     printf("Start X: %d, Start Y: %d, End X: %d, End Y: %d      \n", state , other, under, complete);
//     setCursorPosition(tempCord.X, tempCord.Y);
// }

// int getStartAndEndCoordinates() {
//     int numCars;
//     printf("How many AEDVs would you like to create?: ");
//     scanf("%i", &numCars);
    
//     COORD tempCoord, holdCoord;
//     car = (Car*)malloc(numCars * sizeof(Car));
    
//     if (car == NULL) {
//         printf("Memory allocation failed.\n");
//         exit(1);
//     }
    
//     int quad;
//     enum QUAD tempQuad;
    
//     for(int i = 0; i < numCars; i++) {
//         printf("Enter the X Y QUAD coordinate for the starting point for car %i: ", i);
//         scanf("%i%i%i", &tempCoord.X, &tempCoord.Y, &quad);

//         tempQuad = mapIntToQuad(quad);

//         holdCoord = convertBuildingCoordToStreet(tempCoord, tempQuad);
        
//         printf("Enter the X Y QUAD coordinate for the ending point for car %i: ", i);
//         scanf("%i%i%i", &tempCoord.X, &tempCoord.Y, &quad);
        
//         tempQuad = mapIntToQuad(quad);

//         tempCoord = convertBuildingCoordToStreet(tempCoord, tempQuad);
//         car[i].x = holdCoord.X;
//         car[i].y = holdCoord.Y;
//         car[i].endPos.X = tempCoord.X;
//         car[i].endPos.Y = tempCoord.Y;

//         debugPrint(car[i].x, car[i].y, car[i].endPos.X, car[i].endPos.Y);
//     }

//     // clear the input buffer
//     while((getchar()) != '\n');
    
//     COORD startOffset = getCursorPosition();
//     for(int i = 0; i < numCars; i++) {
//         car[i].x = car[i].x * 4 + startOffset.X;
//         car[i].y = car[i].y * 4 + startOffset.Y;
//     }
    
//     return numCars;
// }
int getStartAndEndCoordinates() {
    int numCars;
    printf("How many AEDVs would you like to create?: ");
    scanf("%i", &numCars);
    COORD tempCoord[numCars];
    car = (Car*)malloc(numCars * sizeof(Car)+1);
    for(int i = 0; i < numCars; i++)
    {
        printf("Enter the X Y coordinate for the starting point for car %i: ", i);
        scanf("%i%i", &tempCoord[i].X, &tempCoord[i].Y);
        int tempQuad;
        printf("Enter the X Y Quad coordinate for the ending point for car %i: ", i);
        scanf("%i%i%i", &car[i].endPos.X, &car[i].endPos.Y, &tempQuad);
        car[i].endQuad = mapIntToQuad(tempQuad);
    }

    // clear the input buffer
    while((getchar()) != '\n');
    startOffset = getCursorPosition();
    for(int i = 0; i < numCars; i++)
    {
        car[i].x = tempCoord[i].X * 4 + startOffset.X-2;
        car[i].y = tempCoord[i].Y * 4 + startOffset.Y;

        car[i].endPos.X = 4*car[i].endPos.X;
        car[i].endPos.Y = 4*car[i].endPos.Y;
    }
    return numCars;
    
}


void updateEndCoordinates() {
    COORD carPos = getCursorPosition();
    int carNum;
    setCursorPosition(0, 4*ybldg+8);
    printf("Enter the car number you would like to update: ");
    scanf("%d", &carNum);
    printf("Enter the X Y coordinate for the ending point for car %i: ", carNum);
    scanf("%d", &car[carNum].endPos.X);
    scanf("%d", &car[carNum].endPos.Y);

    getchar(); // Clear any remaining newline characters from the input buffer
    setCursorPosition(carPos.X, carPos.Y);
}

// Function to update car's position on the console
void updateCar(CarDirection carDirection, int carNum) {
    COORD prevPos = (COORD){car[carNum].x, car[carNum].y};  // Store the previous position (before updating)
    int attempts = 4; // There are 4 directions. The car will attempt to find a free space in each direction.

    while (attempts > 0) {
        switch (carDirection) {
            case MOVE_UP:
                if (isSpaceFree(car[carNum].x, car[carNum].y - 1)) {
                    car[carNum].y--;
                    attempts = 0; // Stop the loop
                } else {
                    carDirection = MOVE_RIGHT;
                    attempts--;
                }
                break;
            case MOVE_RIGHT:
                if (isSpaceFree(car[carNum].x + 1, car[carNum].y)) {
                    car[carNum].x++;
                    attempts = 0; // Stop the loop
                } else {
                    carDirection = MOVE_DOWN;
                    attempts--;
                }
                break;
            case MOVE_DOWN:
                if (isSpaceFree(car[carNum].x, car[carNum].y + 1)) {
                    car[carNum].y++;
                    attempts = 0; // Stop the loop
                } else {
                    carDirection = MOVE_LEFT;
                    attempts--;
                }
                break;
            case MOVE_LEFT:
                if (isSpaceFree(car[carNum].x - 1, car[carNum].y)) {
                    car[carNum].x--;
                    attempts = 0; // Stop the loop
                } else {
                    carDirection = MOVE_UP;
                    attempts--;
                }
                break;
        }
    }

    setCursorPosition(prevPos.X, prevPos.Y);  // Move cursor to previous position
    printf(" "); // Clear the previous position
    setCursorPosition(car[carNum].x - startOffset.X, car[carNum].y);  // Update for new position
    printf("%i", carNum);
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
void animateCar(int carNum) {
    COORD tempCord;
    static int toggle = 0, count, toggleInside = 0;
    if(car[carNum].x < car[carNum].endPos.X && toggle == 0) {
        updateCar(MOVE_RIGHT, carNum);
        toggleInside = 0;
        //debugPrint("XR");
    }   
    else if(car[carNum].x > car[carNum].endPos.X && toggle == 0) {
        updateCar(MOVE_LEFT, carNum);
        toggleInside = 0;
        //debugPrint("XL");
    }
    else if(car[carNum].y < car[carNum].endPos.Y + startOffset.Y && toggle == 0) {
        updateCar(MOVE_DOWN, carNum);
        toggleInside = 0;
        //debugPrint("YD");
    }
    else if(car[carNum].y > car[carNum].endPos.Y + startOffset.Y && toggle == 0) {
        updateCar(MOVE_UP, carNum);
        toggleInside = 0;
        ///debugPrint("YU");
    }
    else{
        toggle = 1;
        if(car[carNum].endQuad == S)
        {
            if(toggleInside == 0)
            {
                count = 2;
                toggleInside = 1;
            }
            if(count > 0)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else
                toggleInside = 2;
            
            
        }
        if(car[carNum].endQuad == SW)
        {
            if(toggleInside == 0)
            {
                count = 1;
                toggleInside = 1;
            }
            if(count > 0)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else
                toggleInside = 2;
        }
        if(car[carNum].endQuad == W)
        {
            if(toggleInside == 0)
            {
                count = 2;
                toggleInside = 1;
            }
            if(count > 0)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else
                toggleInside = 2;
        }
        if(car[carNum].endQuad == SW)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else
                toggleInside = 2;
        }
        if(car[carNum].endQuad == NW)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0 && toggleInside == 1)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else if(toggleInside == 1)
                toggleInside = 2;
            if(toggleInside == 2)
            {
                count = 2;
                toggleInside = 3;
            }
            if(count > 0 && toggleInside == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else if(toggleInside == 3)
                toggleInside = 4;
        }
            if(car[carNum].endQuad == NE)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0 && toggleInside == 1)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else if(toggleInside == 1)
                toggleInside = 2;
            if(toggleInside == 2)
            {
                count = 4;
                toggleInside = 3;
            }
            if(count > 0 && toggleInside == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else if(toggleInside == 3)
                toggleInside = 4;
            
        }
        if(car[carNum].endQuad == N)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0 && toggleInside == 1)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else if(toggleInside == 1)
                toggleInside = 2;
            if(toggleInside == 2)
            {
                count = 3;
                toggleInside = 3;
            }
            if(count > 0 && toggleInside == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else if(toggleInside == 3)
                toggleInside = 4;
        }
        if(car[carNum].endQuad == NW)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0 && toggleInside == 1)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else if(toggleInside == 1)
                toggleInside = 2;
            if(toggleInside == 2)
            {
                count = 3;
                toggleInside = 3;
            }
            if(count > 0 && toggleInside == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else if(toggleInside == 3)
                toggleInside = 4;
        }
        if(car[carNum].endQuad == W)
        {
            if(toggleInside == 0)
            {
                count = 3;
                toggleInside = 1;
            }
            if(count > 0 && toggleInside == 1)
            {
                updateCar(MOVE_LEFT, carNum);
                count--;
            }
            else if(toggleInside == 1)
                toggleInside = 2;
            if(toggleInside == 2)
            {
                count = 3;
                toggleInside = 3;
            }
            if(count > 0 && toggleInside == 3)
            {
                updateCar(MOVE_UP, carNum);
                count--;
            }
            else if(toggleInside == 3)
                toggleInside = 4;
        }
    }
    Sleep(200);
}

// Function to animate car movement on the console
// void animateCar(int carNum) {
//     COORD tempCord;

//     if(car[carNum].x < car[carNum].endPos.X * 4) {
//         if(((car[carNum].x/4)%2) - s1dir != 0)
//         {
//             updateCar(MOVE_RIGHT, carNum);
//             debugPrint("XR");
//         }
//         else
//         {
//             updateCar(MOVE_DOWN, carNum);
//             debugPrint("YD");
//         }
//     }   
//     else if(car[carNum].x > car[carNum].endPos.X * 4) {
//         if(((car[carNum].x/4)%2) - s1dir != 0)
//         {
//             updateCar(MOVE_LEFT, carNum);
//             debugPrint("XL");
//         }
//         else
//         {
//             updateCar(MOVE_UP, carNum);
//             debugPrint("YU");
//         }
//     }
//     else if(car[carNum].y < car[carNum].endPos.Y * 4 + startOffset.Y) {
//         if(((car[carNum].y/4)%2) - a1dir != 0)
//         {
//             updateCar(MOVE_DOWN, carNum);
//             debugPrint("YD");
//         }
//         else
//         {
//             updateCar(MOVE_RIGHT, carNum);
//             debugPrint("XR");
//         }
//     }
//     else if(car[carNum].y > car[carNum].endPos.Y * 4 + startOffset.Y) {
//         if(((car[carNum].y/4)%2) - a1dir != 0)
//         {
//             updateCar(MOVE_UP, carNum);
//             debugPrint("YU");
//         }
//         else
//         {
//             updateCar(MOVE_LEFT, carNum);
//             debugPrint("XL");
//         }
//     }
//     Sleep(200);
// }

// void animateCar(int carNum)
// {
//     static int state, count = 0, start = 1, undershootIndX = 0, undershootIndY = 0, arrivedStat = 0;
//     int numStepsX = car[carNum].endPos.X * 4 - car[carNum].x, numStepsY = car[carNum].endPos.Y * 4 + startOffset.Y - car[carNum].y;
//     if(count == 0)
//     {
//         if((((car[carNum].x/4)%2) - s1dir) != 0 && start == 1) // has to move up/down 1 to be able to move horiz
//         {
//             state = 0;
//             count = 4;
//         }
//         else // can move in horiz for start
//         {
//             state = 1;
//             if((((car[carNum].endPos.Y/4)%2)+a1dir) != 0) // if dest col is wrong direction
//             {
//                 count = numStepsX;
//                 undershootIndX = 1;
//             }
//             else // if dest col is right direction
//             {
//                 count = numStepsX - 4;
//                 undershootIndX = 0;
//             }
//             start = 0;
//         }
//         if((((car[carNum].endPos.X/4)%2)+s1dir) != 0 && arrivedStat == 1)
//         {
//             state = 2;
//             start = 0;
//             count = numStepsY - 4;
//             undershootIndY = 1;
//         }
//         else if((((car[carNum].endPos.X/4)%2)+a1dir) == 0 && arrivedStat == 1)
//         {
//             state = 2;
//             start = 0;
//             count = numStepsY;
//             undershootIndY = 0;
//         }
//         else if((numStepsX-4*undershootIndX) == 0 && arrivedStat == 1)
//         {
//             state = 2;
//             start = 0;
//             count = numStepsY;
//             undershootIndY = 0;
//         }
//     }
//     //debugPrint(state, arrivedStat, (((car[carNum].endPos.X/4)%2)+s1dir));
//     switch(state){
//         case 0:
//             if((((car[carNum].x/4)%2) - s1dir) != 0)
//             {
//                 updateCar(MOVE_DOWN, carNum);
//                 count--;
//             }
//             else
//             {
//                 updateCar(MOVE_UP, carNum);
//                 count--;
//             }
//             if(count == 0)
//             {
//                 start = 0;
//             }
//             break;
//         case 1:
//             if(numStepsX > 0)
//             {
//                 updateCar(MOVE_RIGHT, carNum);
//                 count--;
//             }
//             else if(numStepsX < 0)
//             {
//                 updateCar(MOVE_LEFT, carNum);
//                 count--;
//             }
//             else
//              arrivedStat = 1;
//             break;
//         case 2:
//             if(numStepsY > 0)
//             {
//                 updateCar(MOVE_DOWN, carNum);
//                 count--;
//             }
//             else if(numStepsY < 0)
//             {
//                 updateCar(MOVE_UP, carNum);
//                 count--;
//             }
//             break;
//         default:
//             break;
//     }
//     Sleep(200);
// }

// void animateCar(int carNum)
// {
//     static int complete = 0, count = 0, oneColShort = 0, state = 0; // state 1 = UP, 2 = DOWN, 3 = RIGHT, 4 = LEFT
//     if(count <= 0)
//     {
//         if((car[carNum].y/4)-s1dir != (car[carNum].endPos.Y/4)-s1dir && complete == 0) // if curent dir is not point towards dest go rowDir by 4
//         {
//             if(car[carNum].x/4 == 1)
//             {
//                 state = 1;
//                 count = 4;
//             }
//             else{
//                 state = 2;
//                 count = 4;
//             }
//         }
//         else if(complete == 0) // if current dir is right go to dest column
//         {
//             if(car[carNum].y/4 == 1)
//             {
//                 state = 4;
//             }
//             else{
//                 state = 3;
//             }
//             if((car[carNum].endPos.X/4)-s1dir != (car[carNum].x/4)-s1dir) // if dest column is wrong dir go 4 less
//             {
//                     oneColShort = 1;
//                     complete = 1;
//             }
//             else{ // otherwise go to dest
//                 oneColShort = 0;
//                 complete = 6;
//             }
//             count = abs(car[carNum].endPos.X * 4 - car[carNum].x) - 4*oneColShort;
//         }
//         else if(complete == 1) 
//         {
//             if(car[carNum].x/4 == 1) // if dest row is wrong dir go 4 more
//             {
//                 state = 1;
//                 printf("here");
//                 count = abs(car[carNum].endPos.Y * 4 + startOffset.Y - car[carNum].y) - 4;
//                 complete = 3;

//             }
//             else{ // if dest row is right dir go to dest row
//                 state = 2;
//                 printf("here2");
//                 count = abs(car[carNum].endPos.Y + startOffset.Y - car[carNum].y);
//                 complete = 4;

//             }
//         }
//         else if(complete == 3) // if dest row was over shot, move over 4 in current row dir
//         {
//             if(car[carNum].y/4 == 1)
//             {
//                 state = 4;
//                 count = 4;
//             }
//             else{
//                 state = 3;
//                 count = 4;
//             }
//             complete = 4;
//         }
//         else if(complete == 4)
//         {
//             if(car[carNum].x/4 == 1) // finally go by 4 in dir and reach destination
//             {
//                 state = 1;
//                 count = 4;
//                 complete = 5;
//             }
//             else{ 
//                 state = 2;
//                 count = 4;
//                 complete = 5;
//             }
//         }
//     }
//     debugPrint(state, count, oneColShort, complete);
//     switch(state)
//     {
//         case 1:
//             updateCar(MOVE_UP, carNum);
//             count--;
//             break;
//         case 2:
//             updateCar(MOVE_DOWN, carNum);
//             count--;
//             break;
//         case 3:
//             updateCar(MOVE_RIGHT, carNum);
//             count--;
//             break;
//         case 4:
//             updateCar(MOVE_LEFT, carNum);
//             count--;
//             break;
//         default:
//             break;
//     }
//     Sleep(200);

// }
// Function to free allocated memory for the city grid
void freeGrid(unsigned int ySize) {
    for (unsigned int i = 0; i < (4 * ySize + 2); i++) {
        free(cityGrid[i]);
    }
    free(cityGrid);
    free(car);
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
    int numCars;
    numCars = getStartAndEndCoordinates();
    hideCursor();
    setConsoleBufferSizeAndWindow(300, 300, 80, 50); // 80 columns wide, 200 rows buffer, 50 rows visible in window
    // Read the file and set up the city grid layout
    read_file();
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
