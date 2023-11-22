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

int compare(const void *a, const void *b) {
    DeliveryRequest *requestA = (DeliveryRequest *)a;
    DeliveryRequest *requestB = (DeliveryRequest *)b;
    return (requestA->time - requestB->time);
}

int main() {
    DeliveryRequest requests[100]; // Assuming we won't have more than 100 requests for simplicity
    int count = 0;
    char input[10];
    
    printf("Delivery Request Entry Program\n");
    while (1) {
        printf("Enter a new delivery request? (yes/no): ");
        scanf("%s", input);
        
        if (strcmp(input, "no") == 0) {
            break;
        }
        
        if (count >= 100) {
            printf("Maximum number of requests reached.\n");
            break;
        }
        
        printf("Enter time (integer): ");
        scanf("%d", &requests[count].time);
        requests[count].event = 'D'; // As per the given spec, the event is always 'D'
        
        printf("Enter origin customer ID: ");
        scanf("%s", requests[count].origin_customer_id);
        
        printf("Enter destination customer ID: ");
        scanf("%s", requests[count].destination_customer_id);
        
        printf("Enter package weight (kg): ");
        scanf("%f", &requests[count].package_weight);
        
        count++;
    }
    
    // Sort the requests by time
    qsort(requests, count, sizeof(DeliveryRequest), compare);
    
    // Write to file
    FILE *file = fopen("Events.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\t%c\t%s\t%s\t%.2f\n", requests[i].time, requests[i].event, 
                requests[i].origin_customer_id, requests[i].destination_customer_id, 
                requests[i].package_weight);
    }
    
    fclose(file);
    printf("Delivery requests saved to Events.txt\n");
    
    return 0;
}
