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
    DeliveryRequest request, header = {0, '0', "0", "0", 0.0}; // Header record
    char input[10] = "yes";
    FILE *file;

    file = fopen("DeliveryRequests.dat", "r+b");
    if (!file) {
        file = fopen("DeliveryRequests.dat", "w+b");
        if (!file) {
            printf("Error opening file!\n");
            return 1;
        }
        fwrite(&header, sizeof(DeliveryRequest), 1, file);
    } else {
        fread(&header, sizeof(DeliveryRequest), 1, file);
    }

    printf("Delivery Request Entry Program\n");

    while (strcmp(input, "no") != 0) {
        printf("Enter time (integer): ");
        scanf("%d", &request.time);
        request.event = 'D';
        
        printf("Enter origin customer ID: ");
        scanf("%s", request.origin_customer_id);
        
        printf("Enter destination customer ID: ");
        scanf("%s", request.destination_customer_id);
        
        printf("Enter package weight (kg): ");
        scanf("%f", &request.package_weight);

        fseek(file, 0, SEEK_END); // Move to the end of the file to append the new request
        fwrite(&request, sizeof(DeliveryRequest), 1, file);

        printf("Enter a new delivery request? (yes/no): ");
        scanf("%s", input);
    }

    fclose(file);
    printf("Delivery requests updated in DeliveryRequests.dat\n");
    return 0;
}
