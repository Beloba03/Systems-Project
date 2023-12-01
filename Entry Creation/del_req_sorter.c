//del_req_sorter.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

char *delimiter = ",\t\n ";

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

int main() {
    FILE *inFile = fopen("Events.txt", "r");
    FILE *outFile = fopen("events_sorted.txt", "w");
    char buffer[MAX_LINE_LENGTH];

    if (inFile == NULL || outFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and discard the header line
    if (!fgets(buffer, sizeof(buffer), inFile)) {
        perror("Error reading header line");
        fclose(inFile);
        fclose(outFile);
        return 1;
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

    printf("Sorted events written to events_sorted.txt. Total records processed: %d\n", recordCount);

    return 0;
}
