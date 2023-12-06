/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This module contains file handling functions
*/

#include "car_header.h"
#include "file_handling_header.h"
#include "linked_list_header.h"




COORD getCoord(char x, char y, enum QUAD direction, char stOrPkSpt) {
    COORD newCoord;
    int BUILDING_OFFSET;
    if(stOrPkSpt == 's') // Gets street position
    {
        BUILDING_OFFSET = 3; // Offset from the building center to the entrance
    }
    else // Gets parking spot position
    {
        BUILDING_OFFSET = 2; // Offset from the building center to the entrance
    }
    
    // Map input coordinates AA, AB, AC, etc., to actual values
    newCoord.X = (x - 'A') * 6 + 4; // 'A' maps to 4, 'B' to 10, 'C' to 16, and so on...
    newCoord.Y = (y - 'A') * 6 + 4; // Same mapping for y

    // Apply direction to the mapped coordinates
    switch (direction) {
        case NE: 
            newCoord.X += 1; 
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case N:  
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case NW: 
            newCoord.X -= 1; 
            newCoord.Y -= BUILDING_OFFSET; 
            break;
        case E:  
            newCoord.X += BUILDING_OFFSET; 
            break;
        case W:  
            newCoord.X -= BUILDING_OFFSET; 
            break;
        case SE: 
            newCoord.X += 1; 
            newCoord.Y += BUILDING_OFFSET; 
            break;
        case S:  
            newCoord.Y += BUILDING_OFFSET; 
            break;
        case SW: 
            newCoord.X -= 1; 
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
        token = strtok(line, delimiter); // Delimiters are space, tab, and comma
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
CarDirection getEndDirection(enum QUAD endQuad)
{
    switch(endQuad)
    {
        case(N): return MOVE_DOWN;
        case(E): return MOVE_LEFT;
        case(S): return MOVE_UP;
        case(W): return MOVE_RIGHT;
        case(NE): return MOVE_DOWN;
        case(NW): return MOVE_DOWN;
        case(SE): return MOVE_UP;
        case(SW): return MOVE_UP;
    }
}
location getCustDest(int custID) {
    FILE *file = fopen("Customers.dat", "rb");
    location retVal;
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Customer customer;
    fseek(file, sizeof(Customer) * (custID - 1), SEEK_SET);
    fread(&customer, sizeof(Customer), 1, file);
    fclose(file);
    retVal.endPos = getCoord(customer.building[0], customer.building[1], customer.entrance, 's');
    retVal.endDir = getEndDirection(customer.entrance);
    retVal.floorNum = customer.floor;
    return retVal;
}

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
    fgets(buffer, sizeof(buffer), inFile); // Skip header line


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

// Comparator function for qsort.
int compareVehicleRecords(const void *a, const void *b) {
    VehicleRecord *recordA = (VehicleRecord *)a;
    VehicleRecord *recordB = (VehicleRecord *)b;
    return recordA->vin - recordB->vin; // Sorting by VIN
}

int sortVehicles(int end) {
    FILE *inFile = fopen("vehicles.txt", "r");
    if (!inFile) {
        perror("Error opening file");
        return 1;
    }

    char buffer[MAX_LINE_LENGTH];
    int userVins[MAX_VIN_COUNT];
    int numUserVins = 0;

    // Ask the user for VINs
    printf("Enter the VINs you want to process (0 to end):\n");
    int inputVin;
    while (1) {
        scanf("%d", &inputVin);
        if (inputVin == 0) break;
        userVins[numUserVins++] = inputVin;
    }

    // Read and discard the header line.
    if (!fgets(buffer, sizeof(buffer), inFile)) {
        fclose(inFile);
        return 2; // Error reading header line
    }

    VehicleRecord *records = malloc(MAX_VIN_COUNT * sizeof(VehicleRecord));  // Adjust size as needed
    if (records == NULL) {
        fclose(inFile);
        return 3; // Error allocating memory
    }

    int recordCount = 0;

    // Read and parse each line.
    while (fgets(buffer, sizeof(buffer), inFile)) {
        int vin;
        sscanf(buffer, "%d", &vin);  // Assuming VIN is the first field in the line

        // Check if the VIN is in the user's list
        int found = 0;
        for (int i = 0; i < numUserVins; i++) {
            if (vin == userVins[i]) {
                found = 1;
                break;
            }
        }
        if (!found) continue;  // Skip this record if VIN not in user list

        // Parsing and storing the record
        char *token = strtok(buffer, delimiter);
        records[recordCount].vin = atoi(token);

        token = strtok(NULL, delimiter);
        strncpy(records[recordCount].lastStable, token, sizeof(records[recordCount].lastStable));

        token = strtok(NULL, delimiter);
        strncpy(records[recordCount].lastStableQuad, token, sizeof(records[recordCount].lastStableQuad));

        token = strtok(NULL, delimiter);
        records[recordCount].batteryCapacity = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].batteryCharge = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].batteryChargeRate = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].drivingDischargeRate = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].idleDischargeRate = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].moveTime = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].idleTime = atoi(token);

        token = strtok(NULL, delimiter);
        records[recordCount].chargeTime = atoi(token);

        recordCount++;
    }

    numCars = recordCount;
    car = (Car*)malloc((numCars) * sizeof(Car)+1);
    currentEvents = (EventRecord*)malloc((numCars) * sizeof(EventRecord)+1);
    for(int i = 0; i < numCars; i++)
    {
        car[i].locQueue.next = NULL;
        getNextEvent(i);
        car[i].endIntersectionStatus = 3;
    }
    if(car == NULL)
    {
        printf("Error allocating memory for car array");
        exit(EXIT_FAILURE);
    }


    // Sort the records by VIN.
    qsort(records, recordCount, sizeof(VehicleRecord), compareVehicleRecords);

    // Write the sorted records to the new file.
    for (int i = 0; i < recordCount; i++) {
        COORD tempCoord;
        car[i].vehicleRecord.vin = records[i].vin;
        car[i].vehicleRecord.batteryCapacity = records[i].batteryCapacity;
        car[i].vehicleRecord.batteryCharge = records[i].batteryCharge;
        car[i].vehicleRecord.batteryChargeRate = records[i].batteryChargeRate;
        car[i].vehicleRecord.drivingDischargeRate = records[i].drivingDischargeRate;
        car[i].vehicleRecord.idleDischargeRate = records[i].idleDischargeRate;
        car[i].vehicleRecord.moveTime = records[i].moveTime;
        car[i].vehicleRecord.idleTime = records[i].idleTime;
        car[i].vehicleRecord.chargeTime = records[i].chargeTime;

        enum QUAD entPos = getEntranceEnum(records[i].lastStableQuad);;
        tempCoord = getCoord(records[i].lastStable[0], records[i].lastStable[1], entPos, 'p');
        car[i].endDirection = getEndDirection(entPos);
        car[i].x = tempCoord.X;
        car[i].y = tempCoord.Y;
        
    }
    free(records);
    fclose(inFile);
    return 0;
}

EventRecord getCurrentEvent(int reset)
{
    static FILE *file = NULL;
    if (file == NULL) {
        file = fopen("events_sorted.txt", "r");
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }
    }
    if(reset)
    {
        fclose(file);
    }
    EventRecord record;
    char line[MAX_LINE_LENGTH];

    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        record.time = -1;
        return record;
    }

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

    // Do not close the file here
    return record;
}

int getLastPackageNumber(FILE *file) {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize <= HEADER_SIZE) {
        return 500; // Default start number if only header is present
    }

    fseek(file, fileSize - sizeof(DeliveryRecord), SEEK_SET);
    DeliveryRecord lastRecord;
    fread(&lastRecord, sizeof(DeliveryRecord), 1, file);
    return lastRecord.packageNum + 1;
}

typedef struct {
    int customerID;
    long recordPosition;
} CustomerIndex;

void updateCustomerIndex(const char* indexFilename, int customerID, long position) {
    FILE *indexFile = fopen(indexFilename, "rb+");
    if (!indexFile) {
        indexFile = fopen(indexFilename, "wb");
        if (!indexFile) {
            perror("Error opening index file");
            return;
        }

        // Write a header record (filled with zeros) when creating the file for the first time
        CustomerIndex header = {0};
        fwrite(&header, sizeof(CustomerIndex), 1, indexFile);
        fclose(indexFile);
        indexFile = fopen(indexFilename, "rb+");
    }

    CustomerIndex entry;
    int found = 0;

    // Search for existing entry
    while (fread(&entry, sizeof(CustomerIndex), 1, indexFile) == 1) {
        if (entry.customerID == customerID) {
            found = 1;
            break;
        }
    }

    if (found) {
        // Update existing entry
        fseek(indexFile, -sizeof(CustomerIndex), SEEK_CUR);
    } else {
        // Add new entry at the end
        fseek(indexFile, 0, SEEK_END);
    }

    entry.customerID = customerID;
    entry.recordPosition = position;
    fwrite(&entry, sizeof(CustomerIndex), 1, indexFile);

    fclose(indexFile);
}


long getPreviousSenderPosition(const char* indexFilename, int customerID) {
    FILE *indexFile = fopen(indexFilename, "rb");
    if (!indexFile) {
        return -1; // Index file doesn't exist or can't be opened
    }

    CustomerIndex entry;
    while (fread(&entry, sizeof(CustomerIndex), 1, indexFile) == 1) {
        if (entry.customerID == customerID) {
            fclose(indexFile);
            return entry.recordPosition;
        }
    }

    fclose(indexFile);
    return -1;  // No matching record found
}


void saveDelInfo(int carNum) {
    FILE *file = fopen("delivery_record.dat", "rb+");
    if (!file) {
        file = fopen("delivery_record.dat", "wb");
        if (!file) {
            perror("Error opening file");
            return;
        }
        DeliveryRecord header = {0};
        fwrite(&header, sizeof(DeliveryRecord), 1, file);
        fclose(file);
        file = fopen("delivery_record.dat", "rb+");
    }

    // Get the position of the last record for this sender
    long prevPos = getPreviousSenderPosition("customer_index.dat", currentEvents[carNum].origin_customer_id);

    DeliveryRecord newRecord = {
        .packageNum = getLastPackageNumber(file),
        .time = currentEvents[carNum].time,
        .event = currentEvents[carNum].event,
        .originCustomerID = currentEvents[carNum].origin_customer_id,
        .destinationCustomerID = currentEvents[carNum].destination_customer_id,
        .packageWeight = currentEvents[carNum].package_weight,
        .deliveryTime = tickTime,
        .prevSameSenderPos = prevPos
    };

    fseek(file, 0, SEEK_END);
    long newRecordPos = ftell(file);
    fwrite(&newRecord, sizeof(DeliveryRecord), 1, file);

    updateCustomerIndex("customer_index.dat", newRecord.originCustomerID, newRecordPos);

    fclose(file);
}
