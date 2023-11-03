#include "BldgGen.h"

unsigned int trigger = 0;

// Declare a variable of type Car
Car *car;

int isSpaceFree(int x, int y) {
    if (cityGrid[y-startOffset.Y][x] == 'C' || cityGrid[y-startOffset.Y][x] == 'S' || cityGrid[y-startOffset.Y][x] == 'B' || cityGrid[y-startOffset.Y][x] == '#' || cityGrid[y-startOffset.Y][x] == 'O') {
        return 0; // Space is not free (occupied by a building)
    }
    return 1; // Space is free
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


#ifdef DEBUG
 void debugPrint(int state, int other, int under, int complete) {
     COORD tempCord = getCursorPosition();
     setCursorPosition(0, 4*ybldg+8);
     printf("Start X: %d, Start Y: %d, End X: %d, End Y: %d      \n", state , other, under, complete);
     setCursorPosition(tempCord.X, tempCord.Y);
 }
#endif

int getStartAndEndCoordinates() {
    int numCars;
    char quadString[5];
    printf("How many AEDVs would you like to create?: ");
    scanf("%i", &numCars);
    COORD tempCoord[numCars];
    car = (Car*)malloc(numCars * sizeof(Car)+1);
    for(int i = 0; i < numCars; i++)
    {
        printf("Enter the X, Y coordinate for the starting point for car %i: ", i);
        scanf("%i%i", &tempCoord[i].X, &tempCoord[i].Y);
        int tempQuad;
        printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", i);
        scanf("%i%i%s", &car[i].endPos.X, &car[i].endPos.Y, &quadString);
        if(strcmp(quadString, "N") == 0)
            tempQuad = 1;
        else if(strcmp(quadString, "NW") == 0)
            tempQuad = 2;
        else if(strcmp(quadString, "W") == 0)
            tempQuad = 3;
        else if(strcmp(quadString, "SW") == 0)
            tempQuad = 4;
        else if(strcmp(quadString, "S") == 0)
            tempQuad = 5;
        else if(strcmp(quadString, "SE") == 0)
            tempQuad = 6;
        else if(strcmp(quadString, "E") == 0)
            tempQuad = 7;
        else if(strcmp(quadString, "NE") == 0)
            tempQuad = 8;
        else
        {
            printf("Invalid quadrant entered");
            getchar();
            exit(EXIT_FAILURE);
        }
        car[i].endQuad = mapIntToQuad(tempQuad);
    }

    // clear the input buffer
    while((getchar()) != '\n');
    startOffset = getCursorPosition();
    for(int i = 0; i < numCars; i++)
    {
        car[i].x = tempCoord[i].X * 4 + startOffset.X;
        car[i].y = tempCoord[i].Y * 4 + startOffset.Y;

        car[i].endPos.X = 4*car[i].endPos.X;
        car[i].endPos.Y = 4*car[i].endPos.Y;
    }
    return numCars;
    
}
void updateEndCoordinates() {
    int numCar;
    COORD tempCord = getCursorPosition();
    setCursorPosition(0, 4*ybldg+8);
    printf("Enter the car num you would like to change: ");
    scanf("%i", &numCar);
    int tempQuad;
    char quadString[4];
    printf("Enter the X, Y, Quad(N,E,S,...) coordinate for the ending point for car %i: ", numCar);
    scanf("%i%i%s", &car[numCar].endPos.X, &car[numCar].endPos.Y, &quadString);
    if(strcmp(quadString, "N") == 0)
        tempQuad = 1;
    else if(strcmp(quadString, "NW") == 0)
        tempQuad = 2;
    else if(strcmp(quadString, "W") == 0)
        tempQuad = 3;
    else if(strcmp(quadString, "SW") == 0)
        tempQuad = 4;
    else if(strcmp(quadString, "S") == 0)
        tempQuad = 5;
    else if(strcmp(quadString, "SE") == 0)
        tempQuad = 6;
    else if(strcmp(quadString, "E") == 0)
        tempQuad = 7;
    else if(strcmp(quadString, "NE") == 0)
        tempQuad = 8;
    else
    {
        printf("Invalid quadrant entered");
        getchar();
        exit(EXIT_FAILURE);
    }
    car[numCar].endQuad = mapIntToQuad(tempQuad);
    car[numCar].endPos.X = 4*car[numCar].endPos.X;
    car[numCar].endPos.Y = 4*car[numCar].endPos.Y;
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

void animateCar(int carNum) {
    COORD tempCord;
    static int toggle[4] = {0, 0 ,0, 0}, count[4], toggleInside[4] = {0, 0 ,0, 0};

    if(trigger != 0)
    {
        toggle[trigger-1] = 0;
        count[trigger-1] = 0;
        toggleInside[trigger-1] = 0;
        trigger = 0;
    }
    if(car[carNum].x < car[carNum].endPos.X && toggle[carNum] == 0) {
        updateCar(MOVE_RIGHT, carNum);
        toggleInside[carNum] = 0;
    }   
    else if(car[carNum].x > car[carNum].endPos.X && toggle[carNum] == 0) {
        updateCar(MOVE_LEFT, carNum);
        toggleInside[carNum] = 0;
    }
    else if(car[carNum].y < car[carNum].endPos.Y + startOffset.Y && toggle[carNum] == 0) {
        updateCar(MOVE_DOWN, carNum);
        toggleInside[carNum] = 0;
    }
    else if(car[carNum].y > car[carNum].endPos.Y + startOffset.Y && toggle[carNum] == 0) {
        updateCar(MOVE_UP, carNum);
        toggleInside[carNum] = 0;
    }
    // This occurs when the vehicle has reached the dest intersection. It excecutes a move to the correct quadrent
    else{
        toggle[carNum] = 1;
        if(car[carNum].endQuad == S)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 2;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else
                toggleInside[carNum] = 2;
            
            
        }
        if(car[carNum].endQuad == SW)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 1;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else
                toggleInside[carNum] = 2;
        }
        if(car[carNum].endQuad == W)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 2;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0)
            {
                updateCar(MOVE_UP, carNum);
                count[carNum]--;
            }
            else
                toggleInside[carNum] = 2;
        }
        if(car[carNum].endQuad == SE)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 3;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else
                toggleInside[carNum] = 2;
        }
        if(car[carNum].endQuad == NE)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 3;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 1)
            {
                updateCar(MOVE_UP, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 1)
                toggleInside[carNum] = 2;
            if(toggleInside[carNum] == 2)
            {
                count[carNum] = 4;
                toggleInside[carNum] = 3;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 3)
                toggleInside[carNum] = 4;
            
        }
        if(car[carNum].endQuad == N)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 3;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 1)
            {
                updateCar(MOVE_UP, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 1)
                toggleInside[carNum] = 2;
            if(toggleInside[carNum] == 2)
            {
                count[carNum] = 3;
                toggleInside[carNum] = 3;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 3)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 3)
                toggleInside[carNum] = 4;
        }
        if(car[carNum].endQuad == NW)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 3;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 1)
            {
                updateCar(MOVE_UP, carNum);
                count[carNum]--;
            }
        }
        if(car[carNum].endQuad == E)
        {
            if(toggleInside[carNum] == 0)
            {
                count[carNum] = 4;
                toggleInside[carNum] = 1;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 1)
            {
                updateCar(MOVE_LEFT, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 1)
                toggleInside[carNum] = 2;
            if(toggleInside[carNum] == 2)
            {
                count[carNum] = 2;
                toggleInside[carNum] = 3;
            }
            if(count[carNum] > 0 && toggleInside[carNum] == 3)
            {
                updateCar(MOVE_UP, carNum);
                count[carNum]--;
            }
            else if(toggleInside[carNum] == 3)
                toggleInside[carNum] = 4;
        }
    }
    Sleep(200);
}