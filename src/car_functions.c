
/*
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

// Helper function to determine if the car has reached its destination
int hasReachedDestination(int carNum, COORD destination) {
    return (car[carNum].x == destination.X && car[carNum].y == destination.Y);
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

// Function to assign a delivery task to a car
void assignDeliveryToCar(int carNum, COORD pickupCoord, COORD deliveryCoord) {
    if (carNum >= numCars) {
        printf("Invalid car number.\n");
        return;
    }

    car[carNum].pickupPos = pickupCoord;
    car[carNum].deliveryPos = deliveryCoord;
    car[carNum].status = EN_ROUTE_TO_PICKUP;
    printf("Car %d assigned to pickup at (%d, %d) and deliver to (%d, %d)\n",
           carNum, pickupCoord.X, pickupCoord.Y, deliveryCoord.X, deliveryCoord.Y);
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
        
        printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", i);
        scanf("%i%i%s", &car[i].endPos.X, &car[i].endPos.Y, &quadString);
        
        car[i].endQuad = mapInputToQuad(quadString);
    }

    // clear the input buffer
    while((getchar()) != '\n');

    startOffset = getCursorPosition();

    for(int i = 0; i < numCars; i++)
    {
 
        car[i].x = tempCoord[i].X + startOffset.X;
        car[i].y = tempCoord[i].Y + startOffset.Y;

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


// Function to animate the car's movement
void animateCarNew(int carNum) {
    COORD nextDest;
    // Determine the next destination based on the car's status
    if (car[carNum].status == EN_ROUTE_TO_PICKUP) {
        nextDest = car[carNum].pickupPos;
    } else if (car[carNum].status == EN_ROUTE_TO_DELIVERY) {
        nextDest = car[carNum].deliveryPos;
    } else if (car[carNum].status == DELIVERING) {
        car[carNum].status = AVAILABLE; // Set the car to available after delivery
        return; // Skip the rest of the function if the car is just delivering
    } else {
        nextDest = car[carNum].endPos; // Default end position if not delivering
    }

    // Check if the car has reached the next destination
    if (car[carNum].x == nextDest.X && car[carNum].y == nextDest.Y) {
        if (car[carNum].status == EN_ROUTE_TO_PICKUP) {
            car[carNum].status = EN_ROUTE_TO_DELIVERY;
        } else if (car[carNum].status == EN_ROUTE_TO_DELIVERY) {
            car[carNum].status = DELIVERING;
        }
        return; // Exit the function if the destination is reached
    }

    // Your existing logic for car movement
    int dir = greaterOrLess(nextDest.X, car[carNum].x); // Direction from the car to the destination in the x direction
    int gr = greaterOrLess(nextDest.X, car[carNum].x), std = getStDir(nextDest.Y + 3);
    int endpos = nextDest.Y, curpos = car[carNum].y, ything = greaterOrLess(nextDest.Y + 3, car[carNum].y);
    static int count = 0, startInd = 0, shortStopX = 0, shortStopY = 0, passY = 0;

    if((getStDir(car[carNum].y) == dir || getStDir(car[carNum].y) == 0) && shortStopX == 0 && dir != 0) {
        if(dir == 1) 
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1)
            updateCar(MOVE_LEFT, carNum);
        startInd = 1;
        if(greaterOrLess(nextDest.Y, car[carNum].y) == getAvDir(nextDest.X) && (nextDest.X == car[carNum].x + 6 || nextDest.X == car[carNum].x - 6))
            shortStopX = 1;
    }
    else if(greaterOrLess(nextDest.X, car[carNum].x) == 0 && greaterOrLess(nextDest.Y + 3, car[carNum].y) == 0) {
        // Car has reached the end intersection
    }
    else if ((getStDir(car[carNum].y) != dir || getStDir(car[carNum].y) == 0) && startInd == 0) {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        count++;
    }
    else if(car[carNum].y != nextDest.Y + 3 && shortStopY == 0) {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        
        if(greaterOrLess(nextDest.X, car[carNum].x) == getStDir(nextDest.Y + 3) && (nextDest.Y + 9 == car[carNum].y || nextDest.Y - 3 == car[carNum].y) && shortStopX == 1 && greaterOrLess(nextDest.X, car[carNum].x) != 0) {
            shortStopY = 1;
            count = 12;
        }
        // Optional debug print
        // debugPrint(car[carNum].y, nextDest.Y, getStDir(car[carNum].y), shortStopY);
    }
    else if(shortStopY == 1) {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        count--;
        if(count == 0) {
            count = 6;
            shortStopY = 2;
        }
    }
    else if(shortStopY == 2) {
        if(dir == 1)
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1)
            updateCar(MOVE_LEFT, carNum);
        count--;
        if(count == 0) {
            count = 6;
            shortStopY = 3;
        }
    }
    else if(shortStopY == 3 && count > 0) {
        if(getAvDir(car[carNum].x) == 1)
            updateCar(MOVE_DOWN, carNum);
        else
            updateCar(MOVE_UP, carNum);
        count--;
    }

    // Sleep(300); // Uncomment if needed for timed animation
}

