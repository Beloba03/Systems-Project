//TestFunc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "src/directional_enums.h"
#define MAX_LINE_LENGTH 256
#define BUILDING_OFFSET 3

char *delimiter = ",\t\n ";


// Structure to define a delivery request
typedef struct DeliveryRequest{
    int time;
    char event;
    char origin_customer_id[50];
    char destination_customer_id[50];
    float package_weight;
} DeliveryRequest;

typedef struct Customer{
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    enum QUAD entrance;
} Customer;
typedef struct {
    int time;
    char event;
    int origin_customer_id;
    int destination_customer_id;
    float package_weight;
} EventRecord;
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

COORD getCustDest(int custID) {
    FILE *file = fopen("Customers.dat", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return (COORD) {-1, -1};
    }
    Customer customer;
    fseek(file, sizeof(Customer) * (custID - 1), SEEK_SET);
    fread(&customer, sizeof(Customer), 1, file);
    fclose(file);
    return getCoord(customer.building[0], customer.building[1], customer.entrance);
}

void setCarDest(int carNum)
{
    FILE *file = fopen("events_sorted.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    EventRecord record;
    char line[MAX_LINE_LENGTH];
    while(fgets(line, MAX_LINE_LENGTH, file)) {
        char *token = strtok(line, delimiter); // Delimiters are space, tab, and comma
        if (token) record.time = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) record.event = token[0];

        token = strtok(NULL, delimiter);
        if (token) record.origin_customer_id = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) record.destination_customer_id = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) record.package_weight = atof(token);
        
        COORD start = getCustDest(record.origin_customer_id);
        COORD end = getCustDest(record.destination_customer_id);
        printf("%i, %i, %i, %i\n",start.X, start.Y, end.X, end.Y);
    }
}

int main()
{
    COORD coord = getCustDest(2);
    printf("%d, %d", coord.X, coord.Y);
    printf("\n");
    setCarDest(2);
    return 0;
}