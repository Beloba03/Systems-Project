/*car_functions.c
Task 5 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
November 3, 2023

This module contains functions relating to the car's initialization ,movement, and animation.
*/

#include "BldgGen.h"
#include "car_header.h"
#define DEBUG // Comment this out to remove debug information
#define MANUAL_ENTRY // Comment this out to remove manual entry of car's start and end coordinates

// Declare a car variable array
Car *car;

// Check if the space is free (is a street)
int isSpaceFree(int x, int y) {
    if (cityGrid[y][x] == 'C' || cityGrid[y][x] == 'S' || cityGrid[y][x] == 'B' || cityGrid[y][x] == '#' || cityGrid[y][x] == 'O') {
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

#ifdef MANUAL_ENTRY
// Gets the start and end coordinates for the cars from the user
int getStartAndEndCoordinates() {
    int maxCars;
    int numCars;
    COORD endPos;
    char quadString[MAX_QUAD_LENGTH];
    printf("How many AEDVs would you like to create?: ");
    scanf("%i", &numCars);
    COORD tempCoord[numCars];
    car = (Car*)malloc(numCars * sizeof(Car)+1);
    maxCars = numCars;
    for(int i = 0; i < numCars; i++)
    {
        car[i].endIntersectionStatus = 0;
        printf("Enter the X, Y coordinate for the starting point for car %i: ", i);
        scanf("%i%i", &tempCoord[i].X, &tempCoord[i].Y);
        printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", i);
        scanf("%i%i%s", &endPos.X, &endPos.Y, &quadString);
    }

    printf("\n");
    // clear the input buffer
    while((getchar()) != '\n');

    startOffset = getCursorPosition();

    for(int i = 0; i < numCars; i++)
    {
        car[i].x = tempCoord[i].X;
        car[i].y = tempCoord[i].Y;
        calcIntersection(endPos.X, endPos.Y, i);
    }
    return numCars;
    
}
#else
void staticCarNum()
{
    int numCars = 1;
    car = (Car*)malloc(numCars * sizeof(Car)+1);
    car[0].endIntersectionStatus = 0;
    if(car == NULL)
    {
        printf("Error allocating memory for car array");
        exit(EXIT_FAILURE);
    }
    return;
}
#endif

// Function to update car's position on the console
void updateCar(CarDirection carDirection, int carNum) {
    COORD prevPos = {car[carNum].x, car[carNum].y + startOffset.Y};  // Store the previous position

    // Define the LOW_CHARGE_THRESHOLD based on your requirements
    float LOW_CHARGE_THRESHOLD = 20.0; // Adjust this value as needed

    // Check if the car's charge is low and it's not already charging
    if (car[carNum].charge <= LOW_CHARGE_THRESHOLD && !car[carNum].isCharging) {
        // Find the nearest charging station
        COORD chargingStation = findNearestChargingStation(car[carNum].x, car[carNum].y);
        // Set the car's destination to the charging station
        // This requires modifying the car's destination or path logic
        car[carNum].isCharging = 1; // Mark the car as going to charge
    }

    int attempts = 4; // There are 4 directions. The car will attempt to find a free space in each direction.
    while (attempts > 0) {
        // Check if the car is idle or moving
        if (car[carNum].isIdle) {
            // Car is idle, consume energy at idle rate
            car[carNum].charge -= IDLE_CONSUMPTION_RATE;
        } else {
            // Car is moving, consume energy at movement rate
            car[carNum].charge -= MOVEMENT_CONSUMPTION_RATE;

            // Additional charge consumed by carried packages
            car[carNum].charge -= PACKAGE_CONSUMPTION_RATE * car[carNum].carriedWeight;
        }

        // Logic to move the car based on the current direction
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

        // Check if the car is at a charging station and recharge if needed
        if (car[carNum].isCharging && cityGrid[car[carNum].y][car[carNum].x] == 'C') {
            car[carNum].charge += CHARGING_RATE;
            if (car[carNum].charge >= MAX_CHARGE) {
                car[carNum].charge = MAX_CHARGE;
                car[carNum].isCharging = 0; // Car is fully charged
                // Set the car's next destination or task here
            }
        }
    }

    // Set cursor position and print car number
    setCursorPosition(prevPos.X, prevPos.Y);  // Move cursor to previous position
    printf(" "); // Clear the previous position
    setCursorPosition(car[carNum].x, car[carNum].y + startOffset.Y);  // Update for new position
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
    if(cityGrid[y][0] == 'R')
        return 1;
    else if(cityGrid[y][0] == '+')
        return 0;
    else if(cityGrid[y][0] == 'L')
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

// 4 car test input: 1 1 55 32 S 7 9 19 19 S 23 19 49 49 S 25 23 3 7 S

// This function animates the car's movement between current location and end intersection
void animateCarNew(int carNum)
{
    if(car[carNum].endIntersectionStatus == 1)
    {
        int xCount = car[carNum].x - car[carNum].endPos.X, yCount = car[carNum].y - car[carNum].endPos.Y, dirX = greaterOrLess(car[carNum].x, car[carNum].endPos.X), dirY = greaterOrLess(car[carNum].y, car[carNum].endPos.Y);
        if(xCount != 0) // Car needs to move in X direction to get to end point
        {
            if(dirX == 1)
                updateCar(MOVE_RIGHT, carNum);
            else if(dirX == -1)
                updateCar(MOVE_LEFT, carNum);
        }
        else if(yCount != 0) // Car needs to move in y direction to get to end point
        {
            if(dirY == 1)
                updateCar(MOVE_DOWN, carNum);
            else if(dirY == -1)
                updateCar(MOVE_UP, carNum);
        }
        else // Car has reached the end point
        {
            car[carNum].endIntersectionStatus = 2;
            return;
        }
    }

    else if(car[carNum].endIntersectionStatus == 0)
    {
        int dir = greaterOrLess(car[carNum].x, car[carNum].endIntersection.X); // Direction of the car from the end intersection in the x direction
        static int count[MAX_CAR_NUM] = {0}, startInd[MAX_CAR_NUM] = {0}, shortStopX[MAX_CAR_NUM] = {0}, shortStopY[MAX_CAR_NUM] = {0}, passY[MAX_CAR_NUM] = {0}, startIndAve[MAX_CAR_NUM] = {0}, runOnce[MAX_CAR_NUM] = {0}; // Static variables to keep track of the car's movement
        if(runOnce[carNum] == 0)
        {
            if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == getAvDir(car[carNum].endIntersection.X) && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
                shortStopX[carNum] = 1;
            if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == getStDir(car[carNum].endIntersection.Y) && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
            {
                shortStopY[carNum] = 1;
                count[carNum] = 12;
            }
            runOnce[carNum] = 1;
        }
        if((getStDir(car[carNum].y) == dir || getStDir(car[carNum].y) == 0) && shortStopX[carNum] == 0 && dir != 0) // Checks if the car is on the correct street (it points towards the end point)
        {
            if(dir == 1) // Move the car in the correct direction
                updateCar(MOVE_RIGHT, carNum);
            else if(dir == -1)
                updateCar(MOVE_LEFT, carNum);
            startInd[carNum] = 1;
            if((getAvDir(car[carNum].x) == greaterOrLess(car[carNum].y, car[carNum].endPos.Y) && getAvDir(car[carNum].x) != 0) && getStDir(car[carNum].y) != 0 && getStDir(car[carNum].y != greaterOrLess(car[carNum].x, car[carNum].endPos.X))) // If the car is on the correct street but the street points in the wrong direction, move the car in the direction of the avenue
                startIndAve[carNum] = 1;
            if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == getAvDir(car[carNum].endIntersection.X) && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
                shortStopX[carNum] = 1;
            if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == getStDir(car[carNum].endIntersection.Y) && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
            {
                shortStopY[carNum] = 1;
                count[carNum] = 12;
            }
        }
        // If in the end col and the direction is wrong: move in row dir to next col
       else if(car[carNum].x == car[carNum].endPos.X && (getAvDir(car[carNum].x) != greaterOrLess(car[carNum].y, car[carNum].endPos.Y) && getAvDir(car[carNum].x) != 0))
        {
            if(getStDir(car[carNum].y) == 1)
                updateCar(MOVE_RIGHT, carNum);
            else if(getStDir(car[carNum].y) == -1)
                updateCar(MOVE_LEFT, carNum);
            else if(getStDir(car[carNum].y) == 0)
            {
                if(getStDir(car[carNum].endIntersection.Y) == -1)
                    updateCar(MOVE_LEFT, carNum);
                else if(getStDir(car[carNum].endIntersection.Y) == 1)
                    updateCar(MOVE_RIGHT, carNum);
            }
        }
        else if(getAvDir(car[carNum].x) == 2 && startIndAve[carNum] == 0) // Move in the direction of the current street
        {
            if(getStDir(car[carNum].y) == 1) // Move the car in the correct direction
                updateCar(MOVE_RIGHT, carNum);
            else if(getStDir(car[carNum].y) == -1)
                updateCar(MOVE_LEFT, carNum);
            if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == getAvDir(car[carNum].endIntersection.X) && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
                shortStopX[carNum] = 1;
            if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == getStDir(car[carNum].endIntersection.Y) && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
            {
                shortStopY[carNum] = 1;
                count[carNum] = 12;
            }
        }
        else if ((getStDir(car[carNum].y) == -dir || getStDir(car[carNum].y) == 0) && startInd[carNum] == 0) // Moves the car along the current avenue until the next street in the right direction (this is if it starts where there is no street or if the street points in the wrong direction)
        {
            if(getAvDir(car[carNum].x) == 1)
                updateCar(MOVE_DOWN, carNum);
            else if(getAvDir(car[carNum].x) == -1)
                updateCar(MOVE_UP, carNum);
            else if(getAvDir(car[carNum].x) == 0)
            {
                if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == 1)
                    updateCar(MOVE_UP, carNum);
                else if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == -1)
                    updateCar(MOVE_DOWN, carNum);
            }
            count[carNum]++;
        }
        else if(car[carNum].y != car[carNum].endIntersection.Y && shortStopY[carNum] == 0) // Move along avenue until the end intersection
        {
            if(getAvDir(car[carNum].x) == 1)
                updateCar(MOVE_DOWN, carNum);
            else if(getAvDir(car[carNum].x) == -1)
                updateCar(MOVE_UP, carNum);
            else if(getAvDir(car[carNum].x) == 0)
            {
                if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == 1)
                    updateCar(MOVE_UP, carNum);
                else if(greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == -1)
                    updateCar(MOVE_DOWN, carNum);
            }
            
            if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == getStDir(car[carNum].endIntersection.Y) && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
            {
                shortStopY[carNum] = 1;
                count[carNum] = 12;
            }
            debugPrint(car[carNum].y, car[carNum].endIntersection.Y, getStDir(car[carNum].y), shortStopY[carNum]);
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
            startIndAve[carNum] = 1;
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
        else if(car[carNum].y == car[carNum].endIntersection.Y)
        {
            shortStopX[carNum] = 0;
            if(dir == 1) // Move the car in the correct direction
                updateCar(MOVE_RIGHT, carNum);
            else if(dir == -1)
                updateCar(MOVE_LEFT, carNum);
        }
    }
    if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == 0 && greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == 0 && car[carNum].endIntersectionStatus == 0) // Checks if the car has reached the end intersection
        {
            car[carNum].endIntersectionStatus = 1;
        }
}

// This function calculates the best intersection for the car to navigate to to reach its end point.
void calcIntersection(int x, int y, int carNum)
{
    if((x-1)%6 == 0) // On an avenue
    {
        for(int i = 0; (y-1)%6 != 0; i++)
        {
            y = y-(getAvDir(x));
        }
    }
    else if((y-1) % 6 == 0) // On a street
    {
        for(int i = 0; (x-1)%6 != 0; i++)
        {
            x = x+(getStDir(y));
        }

    }
    else if((x-1)%6 == 0 && (y-1) % 6 == 0) // On an intersection
    {

    }
    else // Wrong COORD entered
    {

    }
    car[carNum].endIntersection.X = x;
    car[carNum].endIntersection.Y = y;
}

void moveToEntrance(int carNum)
{
    return;
}

// Assuming this is where you initialize each car
void initializeCars() {
    for (int i = 0; i < numCars; i++) {
        car[i].isIdle = 1;   // Set the car to be idle
        car[i].charge = INITIAL_CHARGE; // Set the initial charge
        car[i].carriedWeight = 0;       // Initially, the car is not carrying any cargo
        car[i].isCharging = 0;          // Initially, the car is not charging
    }
}

// Function to find the nearest charging station
COORD findNearestChargingStation(int carX, int carY) {
    COORD nearestStation = {0, 0};
    // Initialize variables to store the minimum distance and its coordinates
    int minDistance = INT_MAX;

    for (int y = 0; y < SCALE_FACTOR * ybldg + LAST_STREET_OFFSET; y++) {
        for (int x = 0; x < SCALE_FACTOR * xbldg + LAST_STREET_OFFSET; x++) {
            if (cityGrid[y][x] == 'C') {
                int distance = abs(carX - x) + abs(carY - y); // Calculate Manhattan distance
                if (distance < minDistance) {
                    minDistance = distance;
                    nearestStation.X = x;
                    nearestStation.Y = y;
                }
            }
        }
    }

    return nearestStation;
}


/// IMPLEMENT BETTER ALGORITHM : dijkstras OR SHORT PATH ALGORITHM