//customer_creation.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 256
typedef enum { NE, N, NW, E, W, SE, S, SW } QUAD;

typedef struct {
    int customer_id;
    char firstName[50];
    char lastName[50];
    char building[4];
    int floor;
    QUAD entrance;
} Customer;

QUAD getEntranceEnum(const char* entrance) {
    if (strcmp(entrance, "N") == 0) return N;
    if (strcmp(entrance, "E") == 0) return E;
    if (strcmp(entrance, "W") == 0) return W;
    if (strcmp(entrance, "S") == 0) return S;
    if (strcmp(entrance, "NE") == 0) return NE;
    if (strcmp(entrance, "NW") == 0) return NW;
    if (strcmp(entrance, "SE") == 0) return SE;
    if (strcmp(entrance, "SW") == 0) return SW;
    return N; // Default case
}

Customer blankCustomer() {
    Customer c;
    c.customer_id = -1;
    memset(c.firstName, 0, sizeof(c.firstName));
    memset(c.lastName, 0, sizeof(c.lastName));
    memset(c.building, 0, sizeof(c.building));
    c.floor = -1;
    c.entrance = N; // Default value
    return c;
}

int main() {
    FILE *inFile = fopen("Customers.txt", "r");
    FILE *outFile = fopen("Customers.dat", "wb");

    if (inFile == NULL || outFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    Customer customer;
    char line[MAX_LINE_LENGTH];
    char *token;
    int expectedID = 1; // Start from ID 1

    int count;
    // Count number of lines that will be needed
    while(fgets(line, MAX_LINE_LENGTH, inFile)) {
        token = strtok(line, " \t,"); // Delimiters are space, tab, and comma
        if (token) count = atoi(token);
    }
    rewind(inFile); // Reset file pointer to the beginning

    // Skip header line
    fgets(line, MAX_LINE_LENGTH, inFile);

    while (fgets(line, MAX_LINE_LENGTH, inFile)) {
        token = strtok(line, " \t,"); // Delimiters are space, tab, and comma
        if (token) customer.customer_id = atoi(token);

        token = strtok(NULL, " \t,");
        if (token) strncpy(customer.firstName, token, sizeof(customer.firstName) - 1);

        token = strtok(NULL, " \t,");
        if (token) strncpy(customer.lastName, token, sizeof(customer.lastName) - 1);

        token = strtok(NULL, " \t,");
        if (token) strncpy(customer.building, token, sizeof(customer.building) - 1);

        token = strtok(NULL, " \t,");
        if (token) customer.entrance = getEntranceEnum(token);

        token = strtok(NULL, " \t,");
        if (token) customer.floor = atoi(token);

        // If there is a gap in the IDs, fill it with blank customers
        while (expectedID < customer.customer_id) {
            Customer blank = blankCustomer();
            blank.customer_id = expectedID; // Assign the sequential ID
            fwrite(&blank, sizeof(Customer), 1, outFile);
            expectedID++;
        }

        // Write the actual customer
        fwrite(&customer, sizeof(Customer), 1, outFile);
        expectedID++; // Move to the next expected ID
    }

    // After all records are processed, check if there are additional IDs to fill
    while (expectedID <= count) {
        Customer blank = blankCustomer();
        blank.customer_id = expectedID;
        fwrite(&blank, sizeof(Customer), 1, outFile);
        expectedID++;
    }

    fclose(inFile);
    fclose(outFile);

    printf("Converted Customers.txt to Customers.dat with sequential IDs.\n");

    return 0;
}
