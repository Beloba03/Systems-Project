/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This module contains functions relating to the car's initialization ,movement, and animation.
*/

#include "BldgGen.h"
#include "car_header.h"
#include "linked_list_header.h"
#define DEBUG // Comment this out to remove debug information
// Declare a car variable array
Car *car;
EventRecord *currentEvents;

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

// Function to update car's position on the console
void updateCar(CarDirection carDirection, int carNum) {
    COORD prevPos = (COORD){car[carNum].x, car[carNum].y+startOffset.Y};  // Store the previous position (before updating)
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
            case STOP:
                attempts = 0; // Stop the loop
                break;
        }
    }

    setCursorPosition(prevPos.X, prevPos.Y);  // Move cursor to previous position
    printf(" "); // Clear the previous position
    setCursorPosition(car[carNum].x, car[carNum].y+startOffset.Y);  // Update for new position
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

// This function calculates the best intersection for the car to navigate to to reach its end point.
void calcIntersection(int x, int y, int carNum)
{
    if((x-1)%6 == 0) // On an avenue
    {
        for(int i = 0; (y-1)%6 != 0; i++)
        {
            int avDir = getAvDir(x);
            if(avDir == 0)
                avDir = greaterOrLess(car[carNum].y, car[carNum].endPos.Y);
            y = y-avDir;
        }
    }
    else if((y-1) % 6 == 0) // On a street
    {
        for(int i = 0; (x-1)%6 != 0; i++)
        {
            int stDir = getStDir(y);
            if(stDir == 0) // If the street is bidirectional, find the direction that points towards the end point
                stDir = greaterOrLess(car[carNum].x, car[carNum].endPos.X);
            if(stDir == 0) // If the current street is bidirectional and the end point is in the same street, guess the direction to get started (better logic should be implemented to get absolutly shortest path)
                stDir = 1;
            x = x-stDir;
        }

    }
    car[carNum].endIntersection.X = x;
    car[carNum].endIntersection.Y = y;
}

CarDirection getOppositeDirection(CarDirection direction)
{
    switch(direction)
    {
        case MOVE_UP:
            return MOVE_DOWN;
        case MOVE_DOWN:
            return MOVE_UP;
        case MOVE_LEFT:
            return MOVE_RIGHT;
        case MOVE_RIGHT:
            return MOVE_LEFT;
    }
    return NONE;
}
void getNextEvent(int carNum)
{
    if(car[carNum].locQueue.next == NULL)
    {
        static int runOnce[MAX_CAR_NUM] = {0};
        if(!runOnce[carNum])
            runOnce[carNum] = 1;
        else
            saveDelInfo(carNum);
        currentEvents[carNum] = getCurrentEvent(0);
        if(currentEvents[carNum].time == -1)
        {
            car[carNum].endIntersectionStatus = 8;
            return;
        }
        car[carNum].endIntersectionStatus = 2;
        enqueue(carNum, getCustDest(currentEvents[carNum].origin_customer_id));
        enqueue(carNum, getCustDest(currentEvents[carNum].destination_customer_id));
        car[carNum].delTime = currentEvents[carNum].time;


    }
    
}
void pathFind(int carNum, int reset)
{
    static int count[MAX_CAR_NUM] = {0}, 
    startInd[MAX_CAR_NUM] = {0}, shortStopX[MAX_CAR_NUM] = {0}, 
    shortStopY[MAX_CAR_NUM] = {0}, passY[MAX_CAR_NUM] = {0}, 
    startIndAve[MAX_CAR_NUM] = {0}, runOnce[MAX_CAR_NUM] = {0}; // Static variables to keep track of the car's movement
    if(reset == 1) // Reset for next parcel
    {
        count[carNum] = 0;
        startInd[carNum] = 0;
        shortStopX[carNum] = 0;
        shortStopY[carNum] = 0;
        passY[carNum] = 0;
        startIndAve[carNum] = 0;
        runOnce[carNum] = 0;
        return;
    }
    int dirY = greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y), // Direction of the car from the end intersection in the y direction
    dir = greaterOrLess(car[carNum].x, car[carNum].endIntersection.X), // Direction of the car from the end intersection in the x direction
    endAvDir = getAvDir(car[carNum].endIntersection.X), // Direction of the end avenue
    curAvDir = getAvDir(car[carNum].x), // Direction of the current avenue
    endStDir = getStDir(car[carNum].endIntersection.Y), // Direction of the end street
    curStDir = getStDir(car[carNum].y); // Direction of the current street
    if(runOnce[carNum] == 0)
    {
        if(dirY == endAvDir && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
        {
            startInd[carNum] = 1;
            shortStopX[carNum] = 1;
        }
        if(-dir == endStDir && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && -dir != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
        {
            shortStopY[carNum] = 1;
            count[carNum] = 12;
        }
        runOnce[carNum] = 1;
    }
    if((curStDir == dir || curStDir == 0) && shortStopX[carNum] == 0 && dir != 0) // Checks if the car is on the correct street (it points towards the end point)
    {
        if(passY[carNum] == 1 || dir == 1) // Right is valid
            updateCar(MOVE_RIGHT, carNum);
        else if(passY[carNum] == -1 || dir == -1) // Left is valid, never can be bidirectional
            updateCar(MOVE_LEFT, carNum);
        startInd[carNum] = 1;
        if((curAvDir == -dirY && curAvDir != 0) && curStDir != 0 && getStDir(car[carNum].y) != -dir) // If the car is on the correct street but the street points in the wrong direction, move the car in the direction of the avenue
            startIndAve[carNum] = 1;
        if(dirY == endAvDir && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
            shortStopX[carNum] = 1;
        if(-dir == endStDir && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && -dir != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
        {
            shortStopY[carNum] = 1;
            count[carNum] = 12;
        }
    }
    // If in the end col and the direction is wrong: move in row dir to next col
    else if(car[carNum].x == car[carNum].endIntersection.X && (curAvDir != -dirY && curAvDir != 0))
    {
        if(curStDir == 1) // Right is valid
            updateCar(MOVE_RIGHT, carNum);
        else if(curStDir == -1) // Left is valid
            updateCar(MOVE_LEFT, carNum);
        else if(curStDir == 0) // Both directions are valid so find the one pointing towards the end intersection
        {
            if(endStDir == 1)
            {
                updateCar(MOVE_LEFT, carNum);
                passY[carNum] = -1;
            }
                
            else if(endStDir == -1)
            {
                updateCar(MOVE_RIGHT, carNum);
                passY[carNum] = 1;
            }
        }
    }
    else if(curAvDir == 2 && startIndAve[carNum] == 0) // Move in the direction of the current street
    {
        if(curStDir == 1) // Right is valid
            updateCar(MOVE_RIGHT, carNum);
        else // Left is valid, never can be bidirectional
            updateCar(MOVE_LEFT, carNum);
        if(dirY == endAvDir && (car[carNum].endIntersection.X == car[carNum].x+6 || car[carNum].endIntersection.X == car[carNum].x-6)) // Checks if the destination avenue points towards the destination. If not, it will stop the car one avenue short.
        {
            startInd[carNum] = 1;
            shortStopX[carNum] = 1;
        }
        if(-dir == endStDir && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && -dir != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
        {
            shortStopY[carNum] = 1;
            count[carNum] = 12;
        }
    }
    else if ((curStDir == -dir || curStDir == 0) && startInd[carNum] == 0) // Moves the car along the current avenue until the next street in the right direction (this is if it starts where there is no street or if the street points in the wrong direction)
    {
        passY[carNum] = 0;
        if(curAvDir == 1) // Down is valid
            updateCar(MOVE_DOWN, carNum);
        else if(curAvDir == -1) // Up is valid
            updateCar(MOVE_UP, carNum);
        else if(curAvDir == 0) // Both directions are valid so find the one pointing towards the end intersection
        {
            if(dirY == 1)
                updateCar(MOVE_UP, carNum);
            else if(dirY == -1)
                updateCar(MOVE_DOWN, carNum);
        }
        count[carNum]++;
    }
    else if(car[carNum].y != car[carNum].endIntersection.Y && shortStopY[carNum] == 0) // Move along avenue until the end intersection
    {
        passY[carNum] = 0;
        if(curAvDir == 1) // Down is valid
            updateCar(MOVE_DOWN, carNum);
        else if(curAvDir == -1) // Up is valid
            updateCar(MOVE_UP, carNum);
        else if(curAvDir == 0) // Both directions are valid so find the one pointing towards the end intersection
        {
            if(dirY == 1)
                updateCar(MOVE_UP, carNum);
            else if(dirY == -1)
                updateCar(MOVE_DOWN, carNum);
        }
        if(-dir == endStDir && (car[carNum].endIntersection.Y+6 == car[carNum].y|| car[carNum].endIntersection.Y-6 == car[carNum].y) && shortStopX[carNum] == 1 && -dir != 0) // Stop 1 block before the end intersection if the end street points in the wrong direction and it isn't in the end avenue
        {
            shortStopY[carNum] = 1;
            count[carNum] = 12;
        }
    }
    else if(shortStopY[carNum] == 1) // Move past the end intersection by one block
    {
        passY[carNum] = 0;
        if(curAvDir == 1) // Down is valid
            updateCar(MOVE_DOWN, carNum);
        else // Up is valid, never can be bidirectional
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
        if(dir == 1) // Right is valid
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1) // Left is valid
            updateCar(MOVE_LEFT, carNum);
        count[carNum]--;
        startIndAve[carNum] = 1;
        if(count[carNum] == 0)
        {
            count[carNum] = 6;
            shortStopY[carNum] = 3;
        }
    }
    else if(shortStopY[carNum] == 3 && count[carNum] > 0) // Move one block towards the end intersection in the avenue direction
    {
        if(curAvDir == 1) // Down is valid
            updateCar(MOVE_DOWN, carNum);
        else // Up is valid, never can be bidirectional
            updateCar(MOVE_UP, carNum);
        count[carNum]--;
    }
    else if(car[carNum].y == car[carNum].endIntersection.Y)
    {
        shortStopX[carNum] = 0;
        if(dir == 1) // Right is valid
            updateCar(MOVE_RIGHT, carNum);
        else if(dir == -1) // Left is valid
            updateCar(MOVE_LEFT, carNum);
    }
}

int checkTime(int carNum, int passTime, int floor, int reset)
{
    static time_t curTime[MAX_CAR_NUM] = {0}; // Static array to store times for each car

    if(reset == 1)
    {
        curTime[carNum] = time(NULL); // Reset the time for the given car number
        return -1; // Return -1 when reset
    }
    else
    {
        // Check if the current time is greater or equal to the stored time plus passTime
        if(time(NULL) >= curTime[carNum] + passTime*floor)
        {
            return 1; // Return 1 if the time has passed
        }
        else
        {
            return 0; // Return 0 if the time has not passed
        }
    }
}

void getNextPos(int carNum)
{
    location newPos = dequeue(carNum);
    car[carNum].endPos.X = newPos.endPos.X;
    car[carNum].endPos.Y = newPos.endPos.Y;
    car[carNum].endDirection = newPos.endDir;
    car[carNum].endFloor = newPos.floorNum;
    calcIntersection(newPos.endPos.X, newPos.endPos.Y, carNum);
}
// This function animates the car's movement between current location and end intersection
void animateCarNew(int carNum)
{
    int test = car[carNum].endIntersectionStatus;
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
            updateCar(car[carNum].endDirection, carNum);
            car[carNum].endIntersectionStatus = 2;
            checkTime(carNum, 0, 0, 1);
            getNextEvent(carNum);
            return;
        }
    }
    else if(car[carNum].endIntersectionStatus == 0)
    {
        pathFind(carNum, 0);
    }
    else if(car[carNum].endIntersectionStatus == 2)
    {
        updateCar(STOP, carNum);
        if(checkTime(carNum, 3, car[carNum].endFloor,0) == 1)
        {
            car[carNum].endIntersectionStatus = 3;
        }
    }
    else if(car[carNum].endIntersectionStatus == 3)
    {
        updateCar(STOP, carNum);
        if(car[carNum].delTime <= tickTime)
        {
            int carTime = car[carNum].delTime;
            int tickTimeTemp = tickTime;
            updateCar(getOppositeDirection(car[carNum].endDirection), carNum);
            car[carNum].endIntersectionStatus = 4;
        }
        
    }
    else if(car[carNum].endIntersectionStatus == 4)
    {
        getNextPos(carNum);
        pathFind(carNum, 1);
        updateCar(STOP, carNum);
        checkTime(carNum, 0, 0, 1);
        car[carNum].endIntersectionStatus = 0;

    }
    else if(car[carNum].endIntersectionStatus == 8)
    {
        updateCar(STOP, carNum);
    }
    if(greaterOrLess(car[carNum].endIntersection.X, car[carNum].x) == 0 && greaterOrLess(car[carNum].endIntersection.Y, car[carNum].y) == 0 && car[carNum].endIntersectionStatus == 0) // Checks if the car has reached the end intersection
        {
            car[carNum].endIntersectionStatus = 1;
            updateCar(STOP, carNum);
        }
}

void printTime(int time)
{
    COORD curPos = getCursorPosition();
    setCursorPosition(0, 0);
    printf("Time: %i", time);
    setCursorPosition(curPos.X, curPos.Y);
}