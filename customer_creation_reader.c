#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NE, N, NW, E, W, SE, S, SW } QUAD;

typedef struct {
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    QUAD entrance;
} Customer;

const char* getEntranceString(QUAD entrance) {
    switch (entrance) {
        case N: return "N";
        case E: return "E";
        case W: return "W";
        case S: return "S";
        case NE: return "NE";
        case NW: return "NW";
        case SE: return "SE";
        case SW: return "SW";
        default: return "?";
    }
}

int main() {
    FILE *file = fopen("Customers.dat", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Customer customer;

    // Read and display each customer record
    printf("Customer Records:\n");
    while (fread(&customer, sizeof(Customer), 1, file) == 1) {
        printf("Customer ID: %d\n", customer.customer_id);
        printf("First Name: %s\n", customer.firstName);
        printf("Last Name: %s\n", customer.lastName);
        printf("Building: %s\n", customer.building);
        printf("Floor: %d\n", customer.floor);
        printf("Entrance: %s\n\n", getEntranceString(customer.entrance));
        
    }

    fclose(file);

    return 0;
}
