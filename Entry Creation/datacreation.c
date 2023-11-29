#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100

typedef struct {
    int time;
    char event;
    char origin_customer_id[50];
    char destination_customer_id[50];
    float package_weight;
} DeliveryRequest;

int compare(const void *a, const void *b) {
    DeliveryRequest *requestA = (DeliveryRequest *)a;
    DeliveryRequest *requestB = (DeliveryRequest *)b;
    return (requestB->time - requestA->time);
}

void insertSorted(DeliveryRequest *requests, int count, DeliveryRequest newRequest) {
    int i = count - 1;
    while (i >= 0 && compare(&requests[i], &newRequest) > 0) {
        requests[i + 1] = requests[i];
        i--;
    }
    requests[i + 1] = newRequest;
}

int main() {
    DeliveryRequest requests[MAX_RECORDS], header = {0, '0', "0", "0", 0.0};
    int count = 0;
    char input[10];
    FILE *file, *tempFile;

    file = fopen("DeliveryRequests.dat", "r+b");
    if (file) {
        fread(&header, sizeof(DeliveryRequest), 1, file); // Read the header
        while(fread(&requests[count], sizeof(DeliveryRequest), 1, file) && count < MAX_RECORDS - 1) {
            count++;
        }
        fclose(file);
    }

    printf("Delivery Request Entry Program\n");
    while (strcmp(input, "no") != 0) {
        if (count >= MAX_RECORDS - 1) {
            printf("Maximum number of requests reached.\n");
            break;
        }

        DeliveryRequest newRequest;
        printf("Enter time (integer): ");
        scanf("%d", &newRequest.time);
        newRequest.event = 'D';
        
        printf("Enter origin customer ID: ");
        scanf("%s", newRequest.origin_customer_id);
        
        printf("Enter destination customer ID: ");
        scanf("%s", newRequest.destination_customer_id);
        
        printf("Enter package weight (kg): ");
        scanf("%f", &newRequest.package_weight);

        insertSorted(requests, count, newRequest);
        printf("Enter a new delivery request? (yes/no): ");
        scanf("%s", input);
        count++;
    }

    // Write the sorted records to a temporary file
    tempFile = fopen("TempDeliveryRequests.dat", "w+b");
    if (!tempFile) {
        printf("Error creating temporary file!\n");
        return 1;
    }
    
    // Write header to the temp file
    fwrite(&header, sizeof(DeliveryRequest), 1, tempFile);

    // Write all records to the temp file
    fwrite(requests, sizeof(DeliveryRequest), count, tempFile);
    fclose(tempFile);

    // Delete the old file and rename the new file
    remove("DeliveryRequests.dat");
    rename("TempDeliveryRequests.dat", "DeliveryRequests.dat");

    printf("Delivery requests updated in DeliveryRequests.dat\n");
    return 0;
}
