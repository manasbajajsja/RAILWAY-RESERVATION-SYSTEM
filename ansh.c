#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct train{
    int train_number;
    char train_name[50];
    char source[50];
    char destination[50];
    int seats;
}train;

#define MAX_TRAINS 100

void add_train(train Trains[], int*);
//void search_train(trian, int);
//void update_train(trian, int);
//void delete_train(trian, int*);
void view_all_trains(train Trains[], int);

train Trains[MAX_TRAINS] = {{101, "InterCity Express", "Mumbai", "Pune", 150}, {102, "Coastal Mail", "Kolkata", "Chennai", 120}, {103, "Mountain Special", "Shimla", "Chandigarh", 110}, {104, "Superfast", "Delhi", "Agra", 100}};

int main() {
    int train_count = 4;
    int choice;
    while (1) {
        printf("\n-----------Admin Menu-----------\n");
        printf("1. Add Train\n");
        printf("2. View All Trains\n");
        printf("3. Search Train\n");
        printf("4. Update Train\n");
        printf("5. Delete Train\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            add_train(Trains, &train_count);
        }
        else if (choice == 2) {
            view_all_trains(Trains, train_count);
        }
        /*else if (choice == 3) {
            search_train(Trains, train_count);
        }
        else if (choice == 4) {
            update_train(Trains, train_count);
        }
        else if (choice == 5) {
            delete_train(Trains, &train_count);
        }*/
        else if (choice == 6) {
            exit(0);
        }
        else {
            printf("Invalid choice!! Enter Choice from (1 to 6)\n");
        }
    }
    return 0;
}
void add_train(train Trains[], int *count) {
    if (*count == MAX_TRAINS) {
        printf("Limit Reached!! No more trains can be added.");
        return;
    }
    printf("Enter Train Number: ");
    scanf("%d", &Trains[*count].train_number);
    getchar();

    printf("Enter Train Name: ");
    fgets(Trains[*count].train_name, sizeof(Trains[*count].train_name), stdin);
    Trains[*count].train_name[strcspn(Trains[*count].train_name, "\n")] = '\0';

    printf("Enter Source: ");
    fgets(Trains[*count].source, sizeof(Trains[*count].source), stdin);
    Trains[*count].source[strcspn(Trains[*count].source, "\n")] = '\0';

    printf("Enter Destination: ");
    fgets(Trains[*count].destination, sizeof(Trains[*count].destination), stdin);
    Trains[*count].destination[strcspn(Trains[*count].destination, "\n")] = '\0';

    printf("Enter Total Seats of Train: ");
    scanf("%d", &Trains[*count].seats);
    (*count)++;
    printf("Train Added Successfully!!\n");
}

void view_all_trains(train Trains[], int count) {
    printf("\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-15s %-15s %-10d\n", Trains[i].train_number, Trains[i].train_name, Trains[i].source, Trains[i].destination, Trains[i].seats);
    }
}
