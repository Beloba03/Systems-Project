/*file_handling.c
Read each entry in the file and check if the time matches the current time. Then look for a car at its destination.
Set that cars destination to the pickup location then delivery location.
*/

#include "car_header.h"
#include "file_handling_header.h"

char *delimiter = ",\t\n ";




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




enum QUAD getEntranceEnum(const char* entrance) {
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

int convCustToRel() {
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
        token = strtok(line, delimiter); // Delimiters are space, tab, and comma
        if (token) customer.customer_id = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) strncpy(customer.firstName, token, sizeof(customer.firstName) - 1);

        token = strtok(NULL, delimiter);
        if (token) strncpy(customer.lastName, token, sizeof(customer.lastName) - 1);

        token = strtok(NULL, delimiter);
        if (token) strncpy(customer.building, token, sizeof(customer.building) - 1);

        token = strtok(NULL, delimiter);
        if (token) customer.entrance = getEntranceEnum(token);

        token = strtok(NULL, delimiter);
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

    return 0;
}

COORD getCustDest(int custID, int quadOveride) {
    FILE *file = fopen("Customers.dat", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Customer customer;
    fseek(file, sizeof(Customer) * (custID - 1), SEEK_SET);
    fread(&customer, sizeof(Customer), 1, file);
    fclose(file);
    return getCoord(customer.building[0], customer.building[1], customer.entrance);
}

typedef struct {
    int time;
    char event;
    int origin_customer_id;
    int destination_customer_id;
    float package_weight;
} EventRecord;

int compareEventRecords(const void *a, const void *b) {
    EventRecord *recordA = (EventRecord *)a;
    EventRecord *recordB = (EventRecord *)b;
    return (recordA->time - recordB->time);
}

int sortEvents() {
    FILE *inFile = fopen("Events.txt", "r");
    FILE *outFile = fopen("events_sorted.txt", "w");
    char buffer[MAX_LINE_LENGTH];

    if (inFile == NULL || outFile == NULL) {
        return 1; // Error opening file
    }

    // Read and discard the header line
    if (!fgets(buffer, sizeof(buffer), inFile)) {
        fclose(inFile);
        fclose(outFile);
        return 2; // Error reading header line
    }
    int numEntries = 0;
    while(fgets(buffer, sizeof(buffer), inFile))
    {
        numEntries++;
    }
    rewind(inFile);

    EventRecord records[numEntries];
    int recordCount = 0;

    // Read and parse each line
    while (fgets(buffer, sizeof(buffer), inFile)) {
        char *token = strtok(buffer, delimiter);
        if (token) records[recordCount].time = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) records[recordCount].event = token[0];

        token = strtok(NULL, delimiter);
        if (token) records[recordCount].origin_customer_id = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) records[recordCount].destination_customer_id = atoi(token);

        token = strtok(NULL, delimiter);
        if (token) records[recordCount].package_weight = atof(token);

        recordCount++;
    }

    // Sort the records by time
    qsort(records, recordCount, sizeof(EventRecord), compareEventRecords);

    // Write the sorted records to the new file
    for (int i = 0; i < recordCount; i++) {
        fprintf(outFile, "%d\t%c\t%d\t%d\t%.2f\n",
                records[i].time,
                records[i].event,
                records[i].origin_customer_id,
                records[i].destination_customer_id,
                records[i].package_weight);
    }

    fclose(inFile);
    fclose(outFile);
    return 0;
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

        COORD start = getCustDest(record.origin_customer_id, 1);
        car[carNum].x = start.X;
        car[carNum].y = start.Y;
        car[carNum].endPos = getCustDest(record.destination_customer_id, 0);
        int endX = car[carNum].endPos.X, endY = car[carNum].endPos.Y, curx = car[carNum].x, cury = car[carNum].y;
        int k = 0;
    }
    calcIntersection(car[carNum].endPos.X, car[carNum].endPos.Y, carNum);
}


