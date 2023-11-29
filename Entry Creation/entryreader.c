#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int time;
    char event;
    char origin_customer_id[50];
    char destination_customer_id[50];
    float package_weight;
} DeliveryRequest;

int main() {
    FILE *file = fopen("DeliveryRequests.dat", "rb");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    DeliveryRequest request;

    printf("Delivery Requests:\n");
    while(fread(&request, sizeof(DeliveryRequest), 1, file)) {
        printf("Time: %d, Event: %c, Origin ID: %s, Destination ID: %s, Weight: %.2f kg\n",
            request.time, request.event, request.origin_customer_id, 
            request.destination_customer_id, request.package_weight);
    }

    fclose(file);
    return 0;
}
