/*
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This module contains functions relating to the car's initialization ,movement, and animation.
*/

#include "BldgGen.h"
#define DEBUG // Comment this out to remove debug information
unsigned int trigger = 0; // This is used to select which car to update the end position of

// Declare a car variable array
Car *car;

// Check if the space is free (is a street)
int isSpaceFree(int x, int y) {
    if (cityGrid[y-startOffset.Y][x] == 'C' || cityGrid[y-startOffset.Y][x] == 'S' || cityGrid[y-startOffset.Y][x] == 'B' || cityGrid[y-startOffset.Y][x] == '#' || cityGrid[y-startOffset.Y][x] == 'O') {
        return 0; // Space is not free (occupied by a building)
    }
    return 1; // Space is free
}

// Function to map integer to QUAD enum
enum QUAD mapInputToQuad(char quadString[MAX_QUAD_LENGTH]) {
    if(strcmp(quadString, "N") == 0)
        return N;
    else if(strcmp(quadString, "NW") == 0)
        return NW;
    else if(strcmp(quadString, "W") == 0)
        return W;
    else if(strcmp(quadString, "SW") == 0)
        return SW;
    else if(strcmp(quadString, "S") == 0)
        return S;
    else if(strcmp(quadString, "SE") == 0)
        return SE;
    else if(strcmp(quadString, "E") == 0)
        return E;
    else if(strcmp(quadString, "NE") == 0)
        return NE;
    else
    {
        while((getchar()) != '\n'); // Clear the input buffer
        printf("Invalid quadrant entered");
        getchar();
        exit(EXIT_FAILURE);
    }
}


// Gets the start and end coordinates for the cars from the user
int getStartAndEndCoordinates() {
    int numCars;
    char quadString[MAX_QUAD_LENGTH];
    printf("How many AEDVs would you like to create?: ");
    scanf("%i", &numCars);
    COORD tempCoord[numCars];
    car = (Car*)malloc(numCars * sizeof(Car)+1);
    for(int i = 0; i < numCars; i++)
    {
        printf("Enter the X, Y coordinate for the starting point for car %i: ", i);
        scanf("%i%i", &tempCoord[i].X, &tempCoord[i].Y);
        // if(tempCoord[i].X > xbldg || tempCoord[i].Y > ybldg) // Check if the user entered a valid coordinate
        // {
        //     printf("Invalid coordinate entered, defaulting to max\n");
        //     tempCoord[i].X = xbldg;
        //     tempCoord[i].Y = ybldg;
        // }
        // if(tempCoord[i].X  < 0 && tempCoord[i].Y < 0) // Check if the user entered a valid coordinate (too low)
        // {
        //     printf("Invalid coordinate entered, defaulting to min\n");
        //     tempCoord[i].X = 1;
        //     tempCoord[i].Y = 1;
        // }
        printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", i);
        scanf("%i%i%s", &car[i].endPos.X, &car[i].endPos.Y, &quadString);
        // if(car[i].endPos.X > xbldg || car[i].endPos.Y > ybldg) // Check if the user entered a valid coordinate (too high)
        // {
        //     printf("Invalid coordinate entered, defaulting to max\n"); 
        //     car[i].endPos.X = xbldg;
        //     car[i].endPos.Y = ybldg;
        // }

        // if(car[i].endPos.X  < 1 && car[i].endPos.Y < 1) // Check if the user entered a valid coordinate (too low)
        // {
        //     printf("Invalid coordinate entered, defaulting to min\n");
        //     car[i].endPos.X = 1;
        //     car[i].endPos.Y = 1;
        // }
        
        car[i].endQuad = mapInputToQuad(quadString);
    }

    // clear the input buffer
    while((getchar()) != '\n');

    startOffset = getCursorPosition();

    for(int i = 0; i < numCars; i++)
    {
        // car[i].x = tempCoord[i].X * SCALE_FACTOR + startOffset.X + 1;
        // car[i].y = tempCoord[i].Y * SCALE_FACTOR + startOffset.Y + 1;
        car[i].x = tempCoord[i].X + startOffset.X;
        car[i].y = tempCoord[i].Y + startOffset.Y;

        // car[i].endPos.X = SCALE_FACTOR*car[i].endPos.X + 1;
        // car[i].endPos.Y = SCALE_FACTOR*car[i].endPos.Y + 1;
    }
    return numCars;
    
}

// Update the end coordinates for a car
void updateEndCoordinates() {
    int numCar;
    COORD tempCord = getCursorPosition();
    setCursorPosition(0, SCALE_FACTOR*ybldg+UPDATE_INPUT_OFFSET);
    printf("Enter the car num you would like to change: ");
    scanf("%i", &numCar);
    char quadString[MAX_QUAD_LENGTH];
    printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", numCar);    
    scanf("%i%i%s", &car[numCar].endPos.X, &car[numCar].endPos.Y, &quadString);
    setCursorPosition(0, SCALE_FACTOR*ybldg+UPDATE_INPUT_OFFSET);
    if(car[numCar].endPos.X > xbldg || car[numCar].endPos.Y > ybldg) // Check if the user entered a valid coordinate (too high)
        {
            printf("Invalid coordinate entered, defaulting to max                                      \n");
            car[numCar].endPos.X = xbldg;
            car[numCar].endPos.Y = ybldg;
        }
    if(car[numCar].endPos.X  < 1 && car[numCar].endPos.Y < 1) // Check if the user entered a valid coordinate (too low)
        {
            printf("Invalid coordinate entered, defaulting to min                                      \n");
            car[numCar].endPos.X = 1;
            car[numCar].endPos.Y = 1;
        }
    printf("                                                                                         \
    \n                                                                                  "); // Clear the lines
    car[numCar].endQuad = mapInputToQuad(quadString);
    car[numCar].endPos.X = SCALE_FACTOR*car[numCar].endPos.X+1;
    car[numCar].endPos.Y = SCALE_FACTOR*car[numCar].endPos.Y+1;

    setCursorPosition(tempCord.X, tempCord.Y);
    trigger = numCar+1;
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

// Debug information for the car's movement
#ifdef DEBUG
void debugPrint(int one, int two, char three, int four)
{
    COORD prevPos = getCursorPosition();
    setCursorPosition(0, SCALE_FACTOR*ybldg+8);
    printf("CurX: %i, EndX: %i, Dir: %c, SSY: %i                  \n", one , two, three, four);
    setCursorPosition(prevPos.X, prevPos.Y);
}
#endif
// This function checks the direction of a street at a given y coordinate
int getStDir(int y)
{
    if(cityGrid[y-3][0] == 'R')
        return 1;
    else if(cityGrid[y-3][0] == '+')
        return 0;
    else if(cityGrid[y-3][0] == 'L')
        return -1;
    else
        return 2;
}
// This function checks the direction of an avenue at a given x coordinate
int getAvDir(int x)
{
    if(cityGrid[0][x] == 'D')
        return 1;
    else if(cityGrid[0][x] == '-')
        return 0;
    else if(cityGrid[0][x] == 'U')
        return -1;
    else
        return 2;
}
// Checks if two coordinates are greater or less than each other
int greaterOrLess(int pos1, int pos2)
{
    if(pos1 < pos2)
        return 1;
    else if (pos1 > pos2)
        return -1;
    else
        return 0;

}
// This function animates the car's movement between current location and end intersection
void animateCarNew(int carNum)
{
    int gr = greaterOrLess(car[carNum].endPos.X, car[carNum].x) , std = getStDir(car[carNum].endPos.Y+3), endpos = car[carNum].endPos.Y, curpos = car[carNum].y, ything = greaterOrLess(car[carNum].endPos.Y+3, car[carNum].y); // For debugging
    int dir = greaterOrLess(car[carNum].x, car[carNum].endPos.X); // Direction of the car from the end intersection in the x direction
    static int count[MAX_CAR_NUM] = {0}, startInd[MAX_CAR_NUM] = {0}, shortStopX[MAX_CAR_NUM] = {0}, shortStopY[MAX_CAR_NUM] = {0}, passY[MAX_CAR_NUM] = {0};
        if((getStDir(car[carNum].y) == dir || getStDir(car[carNum].y) == 0) && shortStopX[carNum] == 0 && dir != 0) // Checks if the car is on the correct street (it points towards the end point)
    {
        if(dir == 1) // Move the car in the correct direction
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1)
            updateCar(MOVE_LEFT, carNum);
        startInd[carNum] = 1;
        if(greaterOrLess(car[carNum].endPos.Y, car[carNum].y) == getAvDir(car[carNum].endPos.X) && (car[carNum].endPos.X == car[carNum].x+6 || car[carNum].endPos.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
            shortStopX[carNum] = 1;
    }
    else if(greaterOrLess(car[carNum].endPos.X, car[carNum].x) == 0 && greaterOrLess(car[carNum].endPos.Y +3, car[carNum].y) == 0) // Checks if the car has reached the end intersection
    {

    }
    else if ((getStDir(car[carNum].y) == !dir || getStDir(car[carNum].y) == 0) && startInd[carNum] == 0) // Moves the car along the current avenue until the next street in the right direction (this is if it starts where there is no street or if the street points in te wrong direction)
    {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        count[carNum]++;
    }
    else if(car[carNum].y != car[carNum].endPos.Y+3 && shortStopY[carNum] == 0) // Move along avenue until the end intersection
    {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        
        if(greaterOrLess(car[carNum].endPos.X, car[carNum].x) == getStDir(car[carNum].endPos.Y+3) && (car[carNum].endPos.Y+9 == car[carNum].y|| car[carNum].endPos.Y-3 == car[carNum].y) && shortStopX[carNum] == 1 && greaterOrLess(car[carNum].endPos.X, car[carNum].x) != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
        {
            shortStopY[carNum] = 1;
            count[carNum] = 12;
        }
        debugPrint(car[carNum].y, car[carNum].endPos.Y, getStDir(car[carNum].y), shortStopY[carNum]);
    }
    else if(shortStopY[carNum] == 1) // Move past the end intersection by one block
    {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
                count[carNum]--;
        if(count[carNum] == 0)
        {
            count[carNum] = 6;
            shortStopY[carNum] = 2;
        }
    }
    else if(shortStopY[carNum] == 2) // Move one block back towards the end intersection in the street direction
    {
        if(dir == 1)
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1)
            updateCar(MOVE_LEFT, carNum);
        count[carNum]--;
        if(count[carNum] == 0)
        {
            count[carNum] = 6;
            shortStopY[carNum] = 3;
        }
    }
    else if(shortStopY[carNum] = 3 && count[carNum] > 0) // Move one block towards the end intersection in the avenue direction
    {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        count[carNum]--;
    }

    Sleep(200);
}

COORD calcIntersection(int x, int y)
{
    if((x-3)%6 == 0)
    {

    }
    else if((y-3) % 6 == 0) // Inbetween 
    {

    }
    else // Wrong COORD entered
    {

    }
}