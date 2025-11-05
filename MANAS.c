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
struct Booking {
    char pnr[32];
    int train_number;
    char passenger_name[50];
    int age;
    char gender[10];
    int seat_no;
    int active;
} Bookings[1000] = {
    {"PNR1001", 101, "Rohit Sharma", 32, "M", 1, 1},
    {"PNR1002", 102, "Neha Singh", 27, "F", 1, 1},
    {"PNR1003", 103, "Amit Patel", 40, "M", 2, 1},
    {"PNR1004", 104, "Priya Verma", 29, "F", 3, 1}
};



int booking_count = 4;

#define MAX_TRAINS 100

void add_train(train Trains[], int*);
//void search_train(trian, int);
//void update_train(trian, int);
//void delete_train(trian, int*);
void view_all_trains(train Trains[], int);
void view_all_bookings() {
    if (booking_count == 0) {
        printf("\nNo bookings available.\n");
        return;
    }

    printf("\n%-10s %-10s %-20s %-5s %-10s %-8s %-10s\n",
           "PNR", "Train No", "Passenger Name", "Age", "Gender", "Seat", "Status");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < booking_count; i++) {
        printf("%-10s %-10d %-20s %-5d %-10s %-8d %-10s\n",
               Bookings[i].pnr,
               Bookings[i].train_number,
               Bookings[i].passenger_name,
               Bookings[i].age,
               Bookings[i].gender,
               Bookings[i].seat_no,
               Bookings[i].active ? "CONFIRMED" : "CANCELLED");
    }
}
int Admin_Menu(train *Trains,int *train_count) {
    int choice;
    while (1) {
        printf("\n-----------Admin Menu-----------\n");
        printf("1. Add Train\n");
        printf("2. View All Trains\n");
        printf("3. Search Train\n");
        printf("4. Update Train\n");
        printf("5. Delete Train\n");
        printf("6. View all bookings\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            add_train(Trains, train_count);
        }
        else if (choice == 2) {
            view_all_trains(Trains, *train_count);
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
            view_all_bookings();
        }
        else if(choice ==7){
            break;
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
void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}
int find_train_index_by_no(train *Trains, int train_count, int train_no) {
    for (int i = 0; i < train_count; i++) {
        if (Trains[i].train_number == train_no) 
        return i;
    }
    return -1;
}
void generate_pnr(char *pnr) {
    int random_num = rand() % 10000;
    sprintf(pnr, "PNR%d", random_num);
}
int next_seat_no_for_train(int train_no, int total_seats) {
    if (total_seats <= 0) return -1;

    for (int s = 1; s <= total_seats; s++) {
        int taken = 0;
        for (int i = 0; i < booking_count; i++) {
            if (Bookings[i].active &&
                Bookings[i].train_number == train_no &&
                Bookings[i].seat_no == s) {
                taken = 1;
                break;
            }
        }
        if (taken==0) return s; 
    }

    return -1; 
}
void show_availability(train *Trains, int train_count) {
    int tnum;
    printf("Enter train number to check availability: ");
    if (scanf("%d", &tnum) != 1) {
        clear_input();
        printf("Invalid input. Returning to user menu.\n");
        return;
    }
    clear_input();

    int idx = find_train_index_by_no(Trains, train_count, tnum);
    if (idx == -1) {
        printf("Train not found.\n");
        return;
    }

    /* count active bookings for the train */
    int booked = 0;
    for (int i = 0; i < booking_count; i++) {
        if (Bookings[i].active && Bookings[i].train_number == tnum) booked++;
    }

    int total = Trains[idx].seats;
    int available = total - booked;

    printf("\nTrain %d - %s (%s -> %s)\n", Trains[idx].train_number, Trains[idx].train_name,
           Trains[idx].source, Trains[idx].destination);
    printf("Total seats   : %d\n", total);
    printf("Booked seats  : %d\n", booked);
    printf("Available seats: %d\n", available);
}
void book_ticket(train *Trains, int train_count) {
    if (booking_count >= 1000) {
        printf("Sorry, booking list is full!\n");
        return;
    }

    int tno;
    printf("Enter train number to book: ");
    if (scanf("%d", &tno) != 1) {
        clear_input();
        printf("Invalid input. Returning to menu.\n");
        return;
    }
    clear_input();

    int idx = find_train_index_by_no(Trains, train_count, tno);
    if (idx == -1) {
        printf("Train not found.\n");
        return;
    }

    int seat = next_seat_no_for_train(tno, Trains[idx].seats);
    if (seat == -1) {
        printf("No seats available on this train.\n");
        return;
    }

    struct Booking b;


    generate_pnr(b.pnr);
    b.train_number = tno;

    printf("Enter passenger name: ");
    fgets(b.passenger_name, sizeof(b.passenger_name), stdin);
    b.passenger_name[strcspn(b.passenger_name, "\n")] = '\0';

    printf("Enter age: ");
    if (scanf("%d", &b.age) != 1) {
        clear_input();
        printf("Invalid age! Booking cancelled.\n");
        return;
    }
    clear_input();

    printf("Enter gender: ");
    fgets(b.gender, sizeof(b.gender), stdin);
    b.gender[strcspn(b.gender, "\n")] = '\0';

    b.seat_no = seat;
    b.active = 1;

    /* append to your anonymous Bookings array */
    Bookings[booking_count] = b;
    booking_count++;

    printf("\n Booking Successful!\n");
    printf("PNR: %s\n", b.pnr);
    printf("Train: %d - %s (%s -> %s)\n",
           Trains[idx].train_number, Trains[idx].train_name,
           Trains[idx].source, Trains[idx].destination);
    printf("Passenger: %s, Age: %d, Gender: %s\n", b.passenger_name, b.age, b.gender);
    printf("Seat No: %d\n", b.seat_no);
}
void cancel_booking(void) {
    if (booking_count == 0) {
        printf("No bookings present.\n");
        return;
    }

    char pnr[32];
    printf("Enter PNR to cancel: ");
    fgets(pnr, sizeof(pnr), stdin);
    pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < booking_count; i++) {
        if (strcmp(Bookings[i].pnr, pnr) == 0) {
            if (!Bookings[i].active) {
                printf("Booking already cancelled.\n");
                return;
            }
            Bookings[i].active = 0;
            printf("Booking with PNR %s cancelled successfully.\n", pnr);
            return;
        }
    }
    printf("PNR not found.\n");
}
void view_booking_by_pnr(void) {
    if (booking_count == 0) {
        printf("No bookings present.\n");
        return;
    }

    char pnr[32];
    printf("Enter PNR to view: ");
    fgets(pnr, sizeof(pnr), stdin);
    pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < booking_count; i++) {
        if (strcmp(Bookings[i].pnr, pnr) == 0) {
            printf("\nPNR: %s\n", Bookings[i].pnr);
            printf("Train No: %d\n", Bookings[i].train_number);
            printf("Passenger: %s, Age: %d, Gender: %s\n",
                   Bookings[i].passenger_name, Bookings[i].age, Bookings[i].gender);
            printf("Seat No: %d\n", Bookings[i].seat_no);
            printf("Status: %s\n", Bookings[i].active ? "CONFIRMED" : "CANCELLED");
            return;
        }
    }

    printf("PNR not found.\n");
}


void user_menu(train *Trains, int train_count) {
    int choice;

    while (1) {
        printf("\n--- USER MENU ---\n");
        printf("1. View All Trains\n");
        printf("2. Check Availability\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. View Booking by PNR\n");
        printf("6. Back to Main Menu\n");
        printf("---------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clear_input();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        clear_input();

        switch (choice) {
            case 1:
                view_all_trains(Trains, train_count);
                break;

            

            case 2:
                show_availability(Trains, train_count);
                break;

            case 3:
                book_ticket(Trains, train_count);
                break;

            case 4:
                cancel_booking();
                break;

            case 5:
                view_booking_by_pnr();
                break;

            case 6:
                printf("Returning to Main Menu...\n");
                return;
                break;

            default:
                printf("Invalid choice! Please enter a number between 1 and 7.\n");
                break;
        }
    }
}


int main(){
    train Trains[MAX_TRAINS] = {{101, "InterCity Express", "Mumbai", "Pune", 150}, {102, "Coastal Mail", "Kolkata", "Chennai", 120}, {103, "Mountain Special", "Shimla", "Chandigarh", 110}, {104, "Superfast", "Delhi", "Agra", 100}};
    int train_count = 4;
    int ch=0;
    while(1){
        printf("Welcome To The Railway Reservation System!!\n");
        printf("1.Admin Menu\n");
        printf("2.User Menu\n");
        printf("3.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        if(ch==1){
            Admin_Menu(Trains,&train_count);
            printf("\n");
        }
        else if(ch==2){
            printf("=========\n");
            user_menu(Trains, train_count);
            printf("=========\n");
        }

        else if(ch==3){
            break;
        }
        else{
            printf("Invalid input\n");
        }
    }
}