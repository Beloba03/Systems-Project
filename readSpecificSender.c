#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int packageNum;
    int time;
    char event;
    int originCustomerID;
    int destinationCustomerID;
    float packageWeight;
    int deliveryTime;
    long prevSameSenderPos; // Pointer to the previous record with the same sender
} DeliveryRecord;

void readSpecificSenderRecords(const char *filename, int senderID) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Go to the last record
    fseek(file, -sizeof(DeliveryRecord), SEEK_END);

    DeliveryRecord record;
    printf("Records for Sender ID %d:\n", senderID);
    printf("Package Num | Time | Event | Origin ID | Destination ID | Weight | Delivery Time\n");
    printf("--------------------------------------------------------------------------------\n");

    while (fread(&record, sizeof(DeliveryRecord), 1, file) == 1) {
        if (record.originCustomerID == senderID) {
            printf("%11d | %4d | %5c | %8d | %14d | %6.2f | %13d\n",
                   record.packageNum, record.time, record.event, record.originCustomerID,
                   record.destinationCustomerID, record.packageWeight, record.deliveryTime);
            if (record.prevSameSenderPos == -1) {
                break; // Reached the first record of this sender
            }
            fseek(file, record.prevSameSenderPos, SEEK_SET); // Go to the previous record of this sender
        } else {
            // Move to the previous record
            fseek(file, -2 * sizeof(DeliveryRecord), SEEK_CUR);
        }
    }

    fclose(file);
}

int main() {
    int senderID;
    printf("Enter sender ID to view records: ");
    scanf("%d", &senderID);
    readSpecificSenderRecords("delivery_record.dat", senderID);
    return 0;
}
