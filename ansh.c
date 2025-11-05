/*Tested your code , its perfect -Rishabh*/
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

void add_train(train *, int*);
void search_train(train *, int);
void update_train(train *, int);
void delete_train(train *, int*);
void view_all_trains(train *, int);

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
        else if (choice == 3) {
            search_train(Trains, train_count);
        }
        else if (choice == 4) {
            update_train(Trains, train_count);
        }
        else if (choice == 5) {
            delete_train(Trains, &train_count);
        }
        else if (choice == 6) {
            exit(0);
        }
        else {
            printf("Invalid choice!! Enter Choice from (1 to 6)\n");
        }
    }
    return 0;
}
void add_train(train *t, int *count) {
    if (*count == MAX_TRAINS) {
        printf("Limit Reached!! No more trains can be added.");
        return;
    }
    printf("Enter Train Number: ");
    scanf("%d", &t[*count].train_number);
    getchar();

    printf("Enter Train Name: ");
    fgets(t[*count].train_name, sizeof(t[*count].train_name), stdin);
    t[*count].train_name[strcspn(t[*count].train_name, "\n")] = '\0';

    printf("Enter Source: ");
    fgets(t[*count].source, sizeof(t[*count].source), stdin);
    t[*count].source[strcspn(t[*count].source, "\n")] = '\0';

    printf("Enter Destination: ");
    fgets(t[*count].destination, sizeof(t[*count].destination), stdin);
    t[*count].destination[strcspn(t[*count].destination, "\n")] = '\0';

    printf("Enter Total Seats of Train: ");
    scanf("%d", &t[*count].seats);
    (*count)++;
    printf("Train Added Successfully!!\n");
}

void view_all_trains(train *t, int count) {
    printf("\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-10d %-20s %-15s %-15s %-10d\n", t[i].train_number, t[i].train_name, t[i].source, t[i].destination, t[i].seats);
    }
}
void search_train(train *t, int count) {
    int t_num;
    int found = 0;
    printf("Enter train number to search it's details: ");
    scanf("%d", &t_num);
    for (int i = 0; i < count; i++) {
        if (t[i].train_number == t_num) {
            found = 1;
            printf("Train Found!!\n");
            printf("Train Number: %d\n", t[i].train_number);
            printf("Train Name: %s\n", t[i].train_name);
            printf("Source: %s\n", t[i].source);
            printf("Destination: %s\n", t[i].destination);
            printf("Total Seats: %d\n", t[i].seats);
        }
    }
    if (found == 1) {
        printf("Train Details Fetched Successfully!!");
    }
    else {
        printf("Train does not exist!!");
    }
}
void update_train(train *t, int count) {
    int t_num;
    int found = 0;
    printf("Enter train number to update it's details: ");
    scanf("%d", &t_num);
    for (int i = 0; i < count; i++) {
        if (t[i].train_number == t_num) {
            found = 1;
            printf("Train Found!!\n");
            printf("Enter New Train Number: ");
            scanf("%d", &t[i].train_number);
            getchar();

            printf("Enter New Train Name: ");
            fgets(t[i].train_name, sizeof(t[i].train_name), stdin);
            t[i].train_name[strcspn(t[i].train_name, "\n")] = '\0';

            printf("Enter New Source: ");
            fgets(t[i].source, sizeof(t[i].source), stdin);
            t[i].source[strcspn(t[i].source, "\n")] = '\0';

            printf("Enter New Destination: ");
            fgets(t[i].destination, sizeof(t[i].destination), stdin);
            t[i].destination[strcspn(t[i].destination, "\n")] = '\0';

            printf("Enter New Total Seats of Train: ");
            scanf("%d", &t[i].seats);
        }
    }
    if (found == 1) {
        printf("Train Details Updatted Successfully!!");
    }
    else {
        printf("Train does not exist!!");
    }
}
void delete_train(train *t, int *count) {
    int t_num;
    int found = 0;
    printf("Enter train number to delete it's details: ");
    scanf("%d", &t_num);
    for (int i = 0; i < *count; i++) {
        if (t[i].train_number == t_num) {
            found = 1;
            for (int j = i; j < *count; j++) {
                t[j] = t[j+1];
            }
            (*count)--;
        }
    }
    if (found == 1) {
        printf("Train Details Deleted Successfully!!");
    }
    else {
        printf("Train does not exist!!");
    }
}
