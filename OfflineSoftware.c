/*
Task 6 - Ben Babineau (B00891217), Aaron Koshy (B00885234)
December 6, 2023

This is the main module for the Offline Software program. It contains the main main function which controls which subprogram to run.
It allows the user to query the delivery record file by bill number or sender ID.
*/

#include "OfflineSoftware.h"

// Function to get the full name of the customer given their ID
void getCustomerNameById(int custID, char *fullName) {
    FILE *file = fopen("Customers.dat", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        strcpy(fullName, "Unknown");
        return;
    }

    Customer customer;
    fseek(file, sizeof(Customer) * (custID - 1), SEEK_SET);
    if (fread(&customer, sizeof(Customer), 1, file) == 1) {
        sprintf(fullName, "%s %s", customer.firstName, customer.lastName);
    } else {
        strcpy(fullName, "Unknown");
    }

    fclose(file);
}

void queryBillNumber(const char *filename, int billNum) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Skip the header record.
    fseek(file, sizeof(DeliveryRecord), SEEK_SET);

    // Calculate the offset from the starting bill number.
    // Since bill numbers start at 500 and the header is one record long,
    // we need to subtract one additional record's worth of bytes.
    int offset = (billNum - 500) * sizeof(DeliveryRecord);
    fseek(file, offset, SEEK_CUR); // Move from current position after header

    DeliveryRecord record;
    if (fread(&record, sizeof(DeliveryRecord), 1, file) == 1) {
        if (record.packageNum == billNum) { // Confirm the bill number matches
            char senderName[101], receiverName[101];
            getCustomerNameById(record.originCustomerID, senderName);
            getCustomerNameById(record.destinationCustomerID, receiverName);

            printf("Bill Number: %d\n", billNum);
            printf("Sender Name: %s\n", senderName);
            printf("Receiver Name: %s\n", receiverName);
            printf("Delivery Time: %d\n", record.deliveryTime);
        } else {
            // If the record's bill number doesn't match, it's likely due to deleted or unused records.
            printf("Record found, but bill number does not match. Possible data inconsistency.\n");
        }
    } else {
        printf("No record found for Bill Number: %d. Ensure the bill number is correct and try again.\n", billNum);
    }

    fclose(file);
}




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
    printf("Package Num | Destination ID\n");
    printf("----------------------------------------------------\n");

    while (fread(&record, sizeof(DeliveryRecord), 1, file) == 1) {
        if (record.originCustomerID == senderID) {
            printf("%5d       | %5d\n",
                   record.packageNum, record.destinationCustomerID);
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

// The main function
int main() {
    char filename[] = "delivery_record.dat";
    int input, packageNum, senderID;
    char fullName[101]; // Buffer to hold the full name

    while (1) {
        printf("\nSelect an option:\n");
        printf("1. Query by bill number\n");
        printf("2. Query by sender ID\n");
        printf("3. Exit\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &input);

        switch (input) {
            case 1:
                printf("Enter bill number to query: ");
                scanf("%d", &packageNum);
                queryBillNumber(filename, packageNum);
                break;
            case 2:
                printf("Enter sender ID to view records: ");
                scanf("%d", &senderID);
                readSpecificSenderRecords(filename, senderID);
                break;
            case 3:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice, please enter 1, 2, or 3.\n");
                break;
        }
    }

    return 0;
}