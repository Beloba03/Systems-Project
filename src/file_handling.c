/*
Read each entry in the file and check if the time matches the current time. Then look for a car at its destination.
Set that cars destination to the pickup location then delivery location.
*/

#include "BldgGen.h"
#include "file_handling_header.h"
void checkForRequests() {
    DeliveryRequest request;
    FILE *file = fopen("DeliveryRequests.dat", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    while(fread(&request, sizeof(DeliveryRequest), 1, file)) {
        if (request.time == time) {
            int i = 0;
            while(car[i].endIntersectionStatus == 2)
            {
                i++;
            }
        }
    }

    fclose(file);
}



COORD getCoord(char x, char y, enum QUAD direction) {
    COORD newCoord;
    
    // Map input coordinates AA, AB, AC, etc., to actual values
    newCoord.X = (x - 'A') * 6 + 4; // 'A' maps to 4, 'B' to 10, 'C' to 16, and so on...
    newCoord.Y = (y - 'A') * 6 + 4; // Same mapping for y

    // Apply direction to the mapped coordinates
    switch (direction) {
        case NE: 
            newCoord.X += BUILDING_OFFSET; 
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case N:  
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case NW: 
            newCoord.X -= BUILDING_OFFSET; 
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case E:  
            newCoord.X += BUILDING_OFFSET; 
            break;
        case W:  
            newCoord.X -= BUILDING_OFFSET; 
            break;
        case SE: 
            newCoord.X += BUILDING_OFFSET; 
            newCoord.Y += BUILDING_OFFSET; 
            break;
        case S:  
            newCoord.Y += BUILDING_OFFSET; 
            break;
        case SW: 
            newCoord.X -= BUILDING_OFFSET; 
            newCoord.Y += BUILDING_OFFSET; 
            break;
        default: // Should never happen
            newCoord.X = -1;
            newCoord.Y = -1;
            break; 
    }

    return newCoord;
}