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
};

#define MAX_TRAINS 100

void add_train(train *, int*,FILE *);
void search_train(train *, int);
void update_train(train *, int,FILE *);
void delete_train(train *, int*,FILE *);
void view_all_trains(train *, int);
void view_all_bookings(int *booking_count,struct Booking *Bookings) {
    if (*booking_count == 0) {
        printf("\nNo bookings available.\n");
        return;
    }

    printf("\n%-10s %-10s %-20s %-5s %-10s %-8s %-10s\n",
           "PNR", "Train No", "Passenger Name", "Age", "Gender", "Seat", "Status");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < *booking_count; i++) {
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
void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}
int Admin_Menu(int *train_count,train *Trains,int *booking_count,struct Booking *Bookings,FILE *fpt) {
    int choice;
    while (1) {
        printf("================================");
        printf("\n-----------Admin Menu-----------\n");
        printf("================================\n");
        printf("1. Add Train\n");
        printf("2. View All Trains\n");
        printf("3. Search Train\n");
        printf("4. Update Train\n");
        printf("5. Delete Train\n");
        printf("6. View All Bookings\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            add_train(Trains, train_count,fpt);
            clear_input();
        }
        else if (choice == 2) {
            view_all_trains(Trains, *train_count);
        }
        else if (choice == 3) {
            search_train(Trains,*train_count);
        }
        else if (choice == 4) {
            update_train(Trains,*train_count,fpt);
            clear_input();
        }
        else if (choice == 5) {
            delete_train(Trains, train_count,fpt);
            clear_input();
        }
        else if(choice==6){
            view_all_bookings(booking_count,Bookings);
        }
        else if (choice == 7) {
            break;
        }
        else {
            printf("Invalid choice!! Enter Choice from (1 to 7)\n");
        }
    }
    return 0;
}
void add_train(train *t, int *count,FILE *fpt) {
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
    fpt=fopen("Trains.txt","w");
    fprintf(fpt,"\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    fprintf(fpt,"---------------------------------------------------------------------------\n");
    for (int i=0;i<*count;i+=1){
        fprintf(fpt,"%-10d %-20s %-15s %-15s %-10d\n", t[i].train_number,
        t[i].train_name, t[i].source, t[i].destination, t[i].seats);
    }
    fclose(fpt);
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
            printf("\nTrain Found!!\n");
            printf("Train Number: %d\n", t[i].train_number);
            printf("Train Name: %s\n", t[i].train_name);
            printf("Source: %s\n", t[i].source);
            printf("Destination: %s\n", t[i].destination);
            printf("Total Seats: %d\n\n", t[i].seats);
        }
    }
    if (found == 1) {
        printf("Train Details Fetched Successfully!!\n");
    }
    else {
        printf("Train does not exist!!\n");
    }
}
void update_train(train *t, int count,FILE *fpt) {
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
        printf("Train Details Updatted Successfully!!\n");
    }
    else {
        printf("Train does not exist!!\n");
    }
    fpt=fopen("Trains.txt","w");
    fprintf(fpt,"\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    fprintf(fpt,"---------------------------------------------------------------------------\n");
    for (int i=0;i<count;i+=1){
        fprintf(fpt,"%-10d %-20s %-15s %-15s %-10d\n", t[i].train_number,
        t[i].train_name, t[i].source, t[i].destination, t[i].seats);
    }
    fclose(fpt);
}
void delete_train(train *t, int *count,FILE *fpt) {
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
        printf("Train Details Deleted Successfully!!\n");
    }
    else {
        printf("Train does not exist!!\n");
    }
    fpt=fopen("Trains.txt","w");
    fprintf(fpt,"\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    fprintf(fpt,"---------------------------------------------------------------------------\n");
    for (int i=0;i<*count;i+=1){
        fprintf(fpt,"%-10d %-20s %-15s %-15s %-10d\n", t[i].train_number,
        t[i].train_name, t[i].source, t[i].destination, t[i].seats);
    }
    fclose(fpt);
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
int next_seat_no_for_train(int train_no, int total_seats,int *booking_count,struct Booking *Bookings) {
    if (total_seats <= 0) return -1;

    for (int s = 1; s <= total_seats; s++) {
        int taken = 0;
        for (int i = 0; i < *booking_count; i++) {
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
void show_availability(train *Trains, int train_count,int *booking_count,struct Booking *Bookings) {
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
    for (int i = 0; i < *booking_count; i++) {
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
void book_ticket(train *Trains, int train_count,int *booking_count,struct Booking *Bookings,FILE *fpu) {
    if (*booking_count >= 1000) {
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

    int seat = next_seat_no_for_train(tno, Trains[idx].seats,booking_count,Bookings);
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
    Bookings[*booking_count] = b;
    *booking_count+=1;

    printf("\n Booking Successful!\n");
    printf("PNR: %s\n", b.pnr);
    printf("Train: %d - %s (%s -> %s)\n",
           Trains[idx].train_number, Trains[idx].train_name,
           Trains[idx].source, Trains[idx].destination);
    printf("Passenger: %s, Age: %d, Gender: %s\n", b.passenger_name, b.age, b.gender);
    printf("Seat No: %d\n", b.seat_no);
    fpu=fopen("User.txt","w");
    for (int i=0;i<*booking_count;i+=1){
        fprintf(fpu,"==========\n");
        fprintf(fpu,"\nPNR: %s\n", Bookings[i].pnr);
        fprintf(fpu,"Train No: %d\n", Bookings[i].train_number);
        fprintf(fpu,"Passenger: %s, Age: %d, Gender: %s\n",
                   Bookings[i].passenger_name, Bookings[i].age, Bookings[i].gender);
        fprintf(fpu,"Seat No: %d\n", Bookings[i].seat_no);
        fprintf(fpu,"Status: %s\n", Bookings[i].active ? "CONFIRMED" : "CANCELLED");
        fprintf(fpu,"==========\n");
    }
    fclose(fpu);
}
void cancel_booking(int *booking_count,struct Booking *Bookings,FILE *fpu) {
    if (*booking_count == 0) {
        printf("No bookings present.\n");
        return;
    }

    char pnr[32];
    printf("Enter PNR to cancel: ");
    fgets(pnr, sizeof(pnr), stdin);
    pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < *booking_count; i++) {
        if (strcmp(Bookings[i].pnr, pnr) == 0) {
            if (!Bookings[i].active) {
                printf("Booking already cancelled.\n");
                return;
            }
            Bookings[i].active = 0;
            printf("Booking with PNR %s cancelled successfully.\n", pnr);
            fpu=fopen("User.txt","w");
            for (int i=0;i<*booking_count;i+=1){
                fprintf(fpu,"==========\n");
                fprintf(fpu,"\nPNR: %s\n", Bookings[i].pnr);
                fprintf(fpu,"Train No: %d\n", Bookings[i].train_number);
                fprintf(fpu,"Passenger: %s, Age: %d, Gender: %s\n",
                           Bookings[i].passenger_name, Bookings[i].age, Bookings[i].gender);
                fprintf(fpu,"Seat No: %d\n", Bookings[i].seat_no);
                fprintf(fpu,"Status: %s\n", Bookings[i].active ? "CONFIRMED" : "CANCELLED");
                fprintf(fpu,"==========\n");
            }
            fclose(fpu);
            return;
        }
    }
    printf("PNR not found.\n");
}
void view_booking_by_pnr(int *booking_count,struct Booking *Bookings) {
    if (*booking_count == 0) {
        printf("No bookings present.\n");
        return;
    }

    char pnr[32];
    printf("Enter PNR to view: ");
    fgets(pnr, sizeof(pnr), stdin);
    pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < *booking_count; i++) {
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

void User_menu(train *Trains, int train_count,int *booking_count,struct Booking *Bookings,FILE *fpu) {
    int choice;

    while (1) {
        printf("=================");
        printf("\n--- USER MENU ---\n");
        printf("=================\n");
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
                show_availability(Trains, train_count,booking_count,Bookings);
                break;

            case 3:
                book_ticket(Trains, train_count,booking_count,Bookings,fpu);
                break;

            case 4:
                cancel_booking(booking_count,Bookings,fpu);
                break;

            case 5:
                view_booking_by_pnr(booking_count,Bookings);
                break;

            case 6:
                printf("Returning to Main Menu...\n");
                return;
                break;

            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
                break;
        }
    }
}
int main(){
    int train_count=4;
    int booking_count=4;
    train Trains[MAX_TRAINS] = {{101, "InterCity Express", "Mumbai", "Pune", 150}, {102, "Coastal Mail", "Kolkata", "Chennai", 120},
    {103, "Mountain Special", "Shimla", "Chandigarh", 110}, {104, "Superfast", "Delhi", "Agra", 100}};
    struct Booking Bookings[1000] = {
    {"PNR1001", 101, "Rohit Sharma", 32, "M", 1, 1},
    {"PNR1002", 102, "Neha Singh", 27, "F", 1, 1},
    {"PNR1003", 103, "Amit Patel", 40, "M", 2, 1},
    {"PNR1004", 104, "Priya Verma", 29, "F", 3, 1}
    };
    FILE *fpt;
    FILE *fpu;
    fpt=fopen("Trains.txt","w");
    if (fpt==NULL){
        printf("no fpt\n");
    }
    fpu=fopen("User.txt","w");
    if (fpu==NULL){
        printf("no fpu\n");
    }
    fprintf(fpt,"\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Total Seats");
    fprintf(fpt,"---------------------------------------------------------------------------\n");
    for (int i=0;i<train_count;i+=1){
        fprintf(fpt,"%-10d %-20s %-15s %-15s %-10d\n", Trains[i].train_number,
        Trains[i].train_name, Trains[i].source, Trains[i].destination, Trains[i].seats);
    }
    for (int i=0;i<booking_count;i+=1){
        fprintf(fpu,"==========\n");
        fprintf(fpu,"\nPNR: %s\n", Bookings[i].pnr);
        fprintf(fpu,"Train No: %d\n", Bookings[i].train_number);
        fprintf(fpu,"Passenger: %s, Age: %d, Gender: %s\n",
                   Bookings[i].passenger_name, Bookings[i].age, Bookings[i].gender);
        fprintf(fpu,"Seat No: %d\n", Bookings[i].seat_no);
        fprintf(fpu,"Status: %s\n", Bookings[i].active ? "CONFIRMED" : "CANCELLED");
        fprintf(fpu,"==========\n");
    }
    fclose(fpt);
    fclose(fpu);
    while (1){
        printf("==========================================\n");
        printf("Welcome to the Railway Reservation System!\n");
        printf("==========================================\n");
        printf("1.Admin Menu\n");
        printf("2.User Menu\n");
        printf("3.Exit\n");
        int ch=0;
        printf("Enter your choice :");
        scanf("%d",&ch);
        if(ch==1){
            Admin_Menu(&train_count,Trains,&booking_count,Bookings,fpt);
        }
        else if(ch==2){
            User_menu(Trains,train_count,&booking_count,Bookings,fpu);
        }
        else if(ch==3){
            printf("==========\n");
            printf("Thank You!\n");
            printf("==========\n");
            break;
        }
        else{
            printf("Wrong Input\n");
        }
    }
    return 0;
}