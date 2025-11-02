/* railway.c
   Simple Railway Reservation System in C
   Features:
    - Persistent storage (binary files for trains and bookings)
    - Admin: add train, view trains
    - User: view trains, check availability, book ticket, cancel ticket, view booking by PNR
    - Generates a simple unique PNR for each booking
    - Seat numbering and basic passenger details
   Compile: gcc railway.c -o railway
   Run: ./railway
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRAIN_FILE "trains.dat"
#define BOOKING_FILE "bookings.dat"
#define MAX_NAME 50
#define MAX_SOURCE 30
#define MAX_DEST 30
#define MAX_TRAINS 100
#define MAX_BOOKINGS 1000

typedef struct {
    int train_no;
    char name[50];
    char source[MAX_SOURCE];
    char dest[MAX_DEST];
    int total_seats;
    int seats_booked; // number currently booked
    float fare; // per seat fare
} Train;

typedef struct {
    char pnr[32];
    int train_no;
    char passenger_name[MAX_NAME];
    int age;
    char gender[10];
    int seat_no;
    float amount_paid;
    time_t booked_time;
    int active; // 1 active (not cancelled), 0 cancelled
} Booking;

/* Utility prototypes */
void press_enter_to_continue();
void clear_stdin();

/* Train file operations */
int load_trains(Train trains[], int max_trains);
int save_trains(Train trains[], int n_trains);

/* Booking file operations */
int load_bookings(Booking bookings[], int max_bookings);
int save_bookings(Booking bookings[], int n_bookings);

/* Application functions */
void init_demo_trains(); // create sample trains if none exist
void admin_menu();
void user_menu();
void view_all_trains(Train trains[], int n_trains);
int find_train_index(Train trains[], int n_trains, int train_no);
void add_train();
void book_ticket();
void view_booking_by_pnr();
void cancel_booking();
void view_all_bookings();
void check_availability();

/* Helpers */
void generate_pnr(char *buf, int bufsize, int train_no);
int next_seat_no_for_train(Booking bookings[], int n_bookings, Train trains[], int n_trains, int train_no);
float compute_fare(Train *t);

/* Main */
int main() {
    // Ensure demo trains exist
    init_demo_trains();

    int choice;
    while (1) {
        printf("\n=== RAILWAY RESERVATION SYSTEM ===\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) { clear_stdin(); choice = 0; }
        clear_stdin();

        if (choice == 1) admin_menu();
        else if (choice == 2) user_menu();
        else if (choice == 3) { printf("Goodbye!\n"); break; }
        else printf("Invalid choice. Try again.\n");
    }
    return 0;
}

/* ---------- Utility functions ---------- */
void press_enter_to_continue() {
    printf("\nPress Enter to continue...");
    getchar();
}
void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* ---------- File IO for trains ---------- */
int load_trains(Train trains[], int max_trains) {
    FILE *fp = fopen(TRAIN_FILE, "rb");
    if (!fp) return 0;
    int count = 0;
    while (count < max_trains && fread(&trains[count], sizeof(Train), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

int save_trains(Train trains[], int n_trains) {
    FILE *fp = fopen(TRAIN_FILE, "wb");
    if (!fp) {
        perror("Failed to open train file for writing");
        return 0;
    }
    fwrite(trains, sizeof(Train), n_trains, fp);
    fclose(fp);
    return n_trains;
}

/* ---------- File IO for bookings ---------- */
int load_bookings(Booking bookings[], int max_bookings) {
    FILE *fp = fopen(BOOKING_FILE, "rb");
    if (!fp) return 0;
    int count = 0;
    while (count < max_bookings && fread(&bookings[count], sizeof(Booking), 1, fp) == 1) {
        count++;
    }
    fclose(fp);
    return count;
}

int save_bookings(Booking bookings[], int n_bookings) {
    FILE *fp = fopen(BOOKING_FILE, "wb");
    if (!fp) {
        perror("Failed to open booking file for writing");
        return 0;
    }
    fwrite(bookings, sizeof(Booking), n_bookings, fp);
    fclose(fp);
    return n_bookings;
}

/* ---------- Demo initialization ---------- */
void init_demo_trains() {
    Train trains[MAX_TRAINS];
    int n = load_trains(trains, MAX_TRAINS);
    if (n > 0) return; // already have trains

    // Create some example trains
    Train t1 = {101, "InterCity Express", "Mumbai", "Pune", 100, 0, 250.0f};
    Train t2 = {102, "Coastal Mail", "Kolkata", "Chennai", 120, 0, 800.0f};
    Train t3 = {103, "Mountain Special", "Shimla", "Chandigarh", 60, 0, 400.0f};
    Train t4 = {104, "Superfast", "Delhi", "Agra", 80, 0, 350.0f};

    Train arr[4];
    arr[0] = t1; arr[1] = t2; arr[2] = t3; arr[3] = t4;
    save_trains(arr, 4);

    // No bookings yet
    Booking empty[0];
    save_bookings(empty, 0);
}

/* ---------- Admin Menu ---------- */
void admin_menu() {
    int choice;
    while (1) {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Train\n");
        printf("2. View All Trains\n");
        printf("3. View All Bookings\n");
        printf("4. Back\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) { clear_stdin(); choice = 0; }
        clear_stdin();

        if (choice == 1) add_train();
        else if (choice == 2) {
            Train trains[MAX_TRAINS];
            int n = load_trains(trains, MAX_TRAINS);
            view_all_trains(trains, n);
            press_enter_to_continue();
        }
        else if (choice == 3) {
            view_all_bookings();
            press_enter_to_continue();
        }
        else if (choice == 4) break;
        else printf("Invalid. Try again.\n");
    }
}

/* ---------- User Menu ---------- */
void user_menu() {
    int choice;
    while (1) {
        printf("\n--- USER MENU ---\n");
        printf("1. View All Trains\n");
        printf("2. Check Availability\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. View Booking (PNR)\n");
        printf("6. Back\n");
        printf("Choose: ");
        if (scanf("%d", &choice) != 1) { clear_stdin(); choice = 0; }
        clear_stdin();

        if (choice == 1) {
            Train trains[MAX_TRAINS];
            int n = load_trains(trains, MAX_TRAINS);
            view_all_trains(trains, n);
            press_enter_to_continue();
        }
        else if (choice == 2) check_availability();
        else if (choice == 3) book_ticket();
        else if (choice == 4) cancel_booking();
        else if (choice == 5) view_booking_by_pnr();
        else if (choice == 6) break;
        else printf("Invalid choice. Try again.\n");
    }
}

/* ---------- Train utilities ---------- */
void view_all_trains(Train trains[], int n_trains) {
    if (n_trains == 0) {
        printf("No trains available.\n");
        return;
    }
    printf("\n%-8s %-20s %-12s %-12s %-8s %-8s %-8s\n",
           "TrainNo", "Name", "Source", "Dest", "Seats", "Booked", "Fare");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < n_trains; i++) {
        printf("%-8d %-20s %-12s %-12s %-8d %-8d %.2f\n",
               trains[i].train_no, trains[i].name, trains[i].source, trains[i].dest,
               trains[i].total_seats, trains[i].seats_booked, trains[i].fare);
    }
}

int find_train_index(Train trains[], int n_trains, int train_no) {
    for (int i = 0; i < n_trains; i++) if (trains[i].train_no == train_no) return i;
    return -1;
}

void add_train() {
    Train trains[MAX_TRAINS];
    int n = load_trains(trains, MAX_TRAINS);

    if (n >= MAX_TRAINS) { printf("Train capacity reached.\n"); return; }

    Train t;
    printf("Enter train number (int): ");
    if (scanf("%d", &t.train_no) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();

    // ensure unique train number
    if (find_train_index(trains, n, t.train_no) != -1) {
        printf("Train number already exists.\n");
        return;
    }

    printf("Enter name: ");
    fgets(t.name, sizeof(t.name), stdin); t.name[strcspn(t.name, "\n")] = '\0';
    printf("Enter source: ");
    fgets(t.source, sizeof(t.source), stdin); t.source[strcspn(t.source, "\n")] = '\0';
    printf("Enter destination: ");
    fgets(t.dest, sizeof(t.dest), stdin); t.dest[strcspn(t.dest, "\n")] = '\0';
    printf("Enter total seats (int): ");
    if (scanf("%d", &t.total_seats) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();
    printf("Enter fare per seat (float): ");
    if (scanf("%f", &t.fare) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();

    t.seats_booked = 0;
    trains[n++] = t;
    save_trains(trains, n);
    printf("Train added successfully.\n");
}

/* ---------- Booking utilities ---------- */
void generate_pnr(char *buf, int bufsize, int train_no) {
    // PNR: T<train_no>-<timestamp>-<rand>
    time_t now = time(NULL);
    int r = rand() % 1000;
    snprintf(buf, bufsize, "T%d-%ld-%03d", train_no, (long)now, r);
}

int next_seat_no_for_train(Booking bookings[], int n_bookings, Train trains[], int n_trains, int train_no) {
    // seat numbers start from 1..total_seats; find smallest free seat
    int idx = find_train_index(trains, n_trains, train_no);
    if (idx == -1) return -1;
    Train *t = &trains[idx];
    int total = t->total_seats;
    // track used seats
    int *used = (int*)calloc(total+1, sizeof(int)); // index 0 unused
    for (int i = 0; i < n_bookings; i++) {
        if (bookings[i].train_no == train_no && bookings[i].active) {
            if (bookings[i].seat_no >=1 && bookings[i].seat_no <= total)
                used[bookings[i].seat_no] = 1;
        }
    }
    int seat = -1;
    for (int s = 1; s <= total; s++) {
        if (!used[s]) { seat = s; break; }
    }
    free(used);
    return seat; // -1 if full
}

float compute_fare(Train *t) {
    // In future we could compute based on age/gender/discounts. For now flat fare.
    return t->fare;
}

void book_ticket() {
    Train trains[MAX_TRAINS];
    Booking bookings[MAX_BOOKINGS];
    int n_trains = load_trains(trains, MAX_TRAINS);
    int n_bookings = load_bookings(bookings, MAX_BOOKINGS);

    if (n_trains == 0) { printf("No trains available.\n"); return; }
    int train_no;
    printf("Enter train number to book: ");
    if (scanf("%d", &train_no) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();

    int t_idx = find_train_index(trains, n_trains, train_no);
    if (t_idx == -1) { printf("Train not found.\n"); return; }
    Train *t = &trains[t_idx];

    if (t->seats_booked >= t->total_seats) { printf("No seats available on this train.\n"); return; }

    Booking b;
    memset(&b, 0, sizeof(Booking));
    generate_pnr(b.pnr, sizeof(b.pnr), train_no);
    b.train_no = train_no;

    printf("Enter passenger name: ");
    fgets(b.passenger_name, sizeof(b.passenger_name), stdin); b.passenger_name[strcspn(b.passenger_name, "\n")] = '\0';
    printf("Enter age: ");
    if (scanf("%d", &b.age) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();
    printf("Enter gender: ");
    fgets(b.gender, sizeof(b.gender), stdin); b.gender[strcspn(b.gender, "\n")] = '\0';

    // find seat
    int seat = next_seat_no_for_train(bookings, n_bookings, trains, n_trains, train_no);
    if (seat == -1) { printf("Booking failed: seats appear full.\n"); return; }
    b.seat_no = seat;
    b.booked_time = time(NULL);
    b.amount_paid = compute_fare(t);
    b.active = 1;

    // append booking
    if (n_bookings >= MAX_BOOKINGS) { printf("Booking storage full.\n"); return; }
    bookings[n_bookings++] = b;

    // update train booked count
    trains[t_idx].seats_booked++;

    // persist
    save_bookings(bookings, n_bookings);
    save_trains(trains, n_trains);

    printf("\nBooking Successful!\n");
    printf("PNR: %s\n", b.pnr);
    printf("Train: %d - %s (%s -> %s)\n", t->train_no, t->name, t->source, t->dest);
    printf("Passenger: %s, Age %d, Gender %s\n", b.passenger_name, b.age, b.gender);
    printf("Seat No: %d\n", b.seat_no);
    printf("Amount Paid: Rs %.2f\n", b.amount_paid);
    press_enter_to_continue();
}

void view_booking_by_pnr() {
    Booking bookings[MAX_BOOKINGS];
    int n_bookings = load_bookings(bookings, MAX_BOOKINGS);
    if (n_bookings == 0) { printf("No bookings.\n"); return; }
    char pnr[64];
    printf("Enter PNR: ");
    fgets(pnr, sizeof(pnr), stdin); pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < n_bookings; i++) {
        if (strcmp(bookings[i].pnr, pnr) == 0) {
            Booking *b = &bookings[i];
            printf("\nPNR: %s\n", b->pnr);
            printf("Train No: %d\n", b->train_no);
            printf("Passenger: %s, Age %d, Gender %s\n", b->passenger_name, b->age, b->gender);
            printf("Seat No: %d\n", b->seat_no);
            printf("Amount Paid: Rs %.2f\n", b->amount_paid);
            printf("Status: %s\n", b->active ? "CONFIRMED" : "CANCELLED");
            char timestr[64];
            struct tm *tm_info = localtime(&b->booked_time);
            strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
            printf("Booked On: %s\n", timestr);
            return;
        }
    }
    printf("PNR not found.\n");
}

void cancel_booking() {
    Booking bookings[MAX_BOOKINGS];
    Train trains[MAX_TRAINS];
    int n_bookings = load_bookings(bookings, MAX_BOOKINGS);
    int n_trains = load_trains(trains, MAX_TRAINS);

    if (n_bookings == 0) { printf("No bookings to cancel.\n"); return; }
    char pnr[64];
    printf("Enter PNR to cancel: ");
    fgets(pnr, sizeof(pnr), stdin); pnr[strcspn(pnr, "\n")] = '\0';

    for (int i = 0; i < n_bookings; i++) {
        if (strcmp(bookings[i].pnr, pnr) == 0) {
            if (!bookings[i].active) { printf("Ticket already cancelled.\n"); return; }
            bookings[i].active = 0;
            // reduce train booked count
            int t_idx = find_train_index(trains, n_trains, bookings[i].train_no);
            if (t_idx != -1 && trains[t_idx].seats_booked > 0) trains[t_idx].seats_booked--;

            save_bookings(bookings, n_bookings);
            save_trains(trains, n_trains);

            printf("Booking cancelled. Refund: Rs %.2f (no cancellation rules applied here)\n", bookings[i].amount_paid);
            return;
        }
    }
    printf("PNR not found.\n");
}

void view_all_bookings() {
    Booking bookings[MAX_BOOKINGS];
    Train trains[MAX_TRAINS];
    int n_bookings = load_bookings(bookings, MAX_BOOKINGS);
    int n_trains = load_trains(trains, MAX_TRAINS);

    if (n_bookings == 0) { printf("No bookings found.\n"); return; }

    printf("\n%-28s %-8s %-20s %-6s %-8s %-10s\n", "PNR", "TrainNo", "Passenger", "Age", "Seat", "Status");
    printf("---------------------------------------------------------------------\n");
    for (int i = 0; i < n_bookings; i++) {
        Booking *b = &bookings[i];
        printf("%-28s %-8d %-20s %-6d %-8d %-10s\n",
               b->pnr, b->train_no, b->passenger_name, b->age, b->seat_no,
               b->active ? "CONFIRMED" : "CANCELLED");
    }
}

/* Check availability for a train */
void check_availability() {
    Train trains[MAX_TRAINS];
    Booking bookings[MAX_BOOKINGS];
    int n_trains = load_trains(trains, MAX_TRAINS);
    int n_bookings = load_bookings(bookings, MAX_BOOKINGS);

    if (n_trains == 0) { printf("No trains available.\n"); return; }
    int train_no;
    printf("Enter train number to check availability: ");
    if (scanf("%d", &train_no) != 1) { clear_stdin(); printf("Bad input.\n"); return; }
    clear_stdin();

    int idx = find_train_index(trains, n_trains, train_no);
    if (idx == -1) { printf("Train not found.\n"); return; }

    Train *t = &trains[idx];
    int available = t->total_seats - t->seats_booked;
    printf("Train %d - %s (%s -> %s)\n", t->train_no, t->name, t->source, t->dest);
    printf("Total seats: %d\n", t->total_seats);
    printf("Booked seats: %d\n", t->seats_booked);
    printf("Available seats: %d\n", available);
}

/* End of file */

