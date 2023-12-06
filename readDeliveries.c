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
    long prevSameSenderPos; // Field to store the position of the previous record with the same sender
} DeliveryRecord;

void readDeliveryRecords(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    DeliveryRecord record;
    printf("Package Num | Time | Event | Origin ID | Destination ID | Weight | Delivery Time | Prev Sender\n");
    printf("--------------------------------------------------------------------------------\n");

    while (fread(&record, sizeof(DeliveryRecord), 1, file) == 1) {
        printf("%11d | %4d | %5c | %8d | %14d | %6.2f | %13d | %d\n",
               record.packageNum, record.time, record.event, record.originCustomerID,
               record.destinationCustomerID, record.packageWeight, record.deliveryTime, record.prevSameSenderPos);
    }

    fclose(file);
}

int main() {
    readDeliveryRecords("delivery_record.dat");
    return 0;
}
