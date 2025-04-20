#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 5
// User constants
#define MIN_PSWRD_LENGTH 8
#define MAX_PSWRD_LENGTH 30
#define ACCOUNT_NUMBER_LENGTH 10
#define MIN_NAME_LENGTH 1
#define MAX_NAME_LENGTH 50
#define MIN_RFC_LENGTH 12
#define MAX_RFC_LENGTH 13
#define MIN_STREET_LENGTH 1
#define MAX_STREET_LENGTH 30
#define MIN_HOUSE_NUMBER_LENGTH 1
#define MAX_HOUSE_NUMBER_LENGTH 10
#define MIN_SUBURB_LENGTH 1
#define MAX_SUBURB_LENGTH 20
#define MIN_CITY_LENGTH 1
#define MAX_CITY_LENGTH 20
#define MIN_PHONE_LENGTH 10
#define MAX_PHONE_LENGTH 13
#define MIN_OPENING_BALANCE 5000.00

typedef char s_string[15]; // A short string for RFC, phone number and other small size data.
typedef char string[50]; // A string for names, surnames and medium size data.

// Clients information (using arrays instead of structs)
s_string account_number[MAX_CLIENTS + 1];
s_string rfc[MAX_CLIENTS + 1];
string name[MAX_CLIENTS + 1];
s_string street[MAX_CLIENTS + 1];
string suburb[MAX_CLIENTS + 1];
string city[MAX_CLIENTS + 1];
s_string house_number[MAX_CLIENTS + 1];
s_string phone[MAX_CLIENTS + 1];
unsigned short registration_day[MAX_CLIENTS + 1];
unsigned short registration_month[MAX_CLIENTS + 1];
unsigned int registration_year[MAX_CLIENTS + 1];
double opening_balance[MAX_CLIENTS + 1];
double current_balance[MAX_CLIENTS + 1];
s_string status;

// Submenus

void administrator_menu(void);
void client_menu(void);

// Administrator options

void register_clients(void);
void remove_clients(void);
void update_information(void);
void client_inquiry(void);

// Client options

void deposit(void);
void withdraw(void);
void check_balance(void);

// Tools

void ask_for_string(const char* item, char* answer, size_t min_length, size_t max_length, bool only_nums);
void remove_newline(char*);

// Debug

void print_log(const char* msg);
void print_clients_and_info(void);


void _ready(void) {
    // Initialize all clients account number to empty strings (to allow checking for empty clients)
    for (size_t i = 0; i == MAX_CLIENTS; i++)
        strcpy(account_number[i], "");
    print_log("Execution started...");
    
}

int main(void) {
    _ready();
    char login_menu;
    string admin = "AntonioLive8166", admin_pswrd = "1", attempt;

    do {
        printf("How do you want to sign in?\n0) Cancel.\n1) Administrator.\n2) Client.\n\n$ ");
        scanf(" %c", &login_menu);
        getchar(); // Clear the input buffer

        system("cls");
        switch (login_menu) {
        case '0': 
            printf("Logging out...\n");
            break;
        
        case '1':
            do {
                printf("Welcome, %s. Enter your password: $ ", admin);
                scanf(" %s", attempt);

                if (strcmp(admin_pswrd, attempt) == 0) {
                    system("cls");
                    administrator_menu();
                } else {
                    system("cls");
                    printf("Wrong password. Please, try again.\n");
                }

            } while (strcmp(admin_pswrd, attempt) != 0);
            break;

        case '2':
            printf("Client functionality not implemented yet.\n");
            break;

        default:
            system("cls");
            printf("Invalid option. Please try again.\n");
            break;
        }

    } while (login_menu != '0');
    
    return 0;
}

void administrator_menu(void) {
    char admin_menu;
    do {
        printf("Administrator Menu:\n0) Back.\n1) Register clients.\n2) Remove clients.\n"
                "3) Update information.\n4) Client inquiry.\n\n$ ");
        scanf(" %c", &admin_menu);
        getchar(); // Clear the input buffer

        system("cls");
        switch (admin_menu) {
        case '0':
            break;
        
        case '1':
            register_clients();
            break;

        case '2':
            remove_clients();
            break;

        case '3':
            update_information();
            break;

        case '4':
            client_inquiry();
            break;

        default:
            printf("Invalid option. Please try again.\n");
            break;
        }

    } while (admin_menu != '0');
}

/** 
 * @brief Looks for an empty slot in clients and fills it with client's data.
*/
/** TODO: Get and store system date */
void register_clients(void) {
    fprintf(stderr, "FIXME: Err. 01 displayed every time the loop finishes, even if there are empty slots.");
    char option;
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        // Check if the client slot is empty to avoid overwriting existing clients.
        if (strcmp(account_number[client], "") == 0) {
            printf("Registering client %zu:\n", client + 1);
            ask_for_string("RFC", rfc[client], MIN_RFC_LENGTH, MAX_RFC_LENGTH, false);
            ask_for_string("name", name[client], MIN_NAME_LENGTH, MAX_NAME_LENGTH, false);
            ask_for_string("street name", street[client], MIN_STREET_LENGTH, MAX_STREET_LENGTH, false);
            ask_for_string("suburb name", suburb[client], MIN_SUBURB_LENGTH, MAX_SUBURB_LENGTH, false);
            ask_for_string("city name", city[client], MIN_CITY_LENGTH, MAX_CITY_LENGTH, false);
            ask_for_string("house number", house_number[client], MIN_HOUSE_NUMBER_LENGTH, MAX_HOUSE_NUMBER_LENGTH, true);
            ask_for_string("phone number", phone[client], MIN_PHONE_LENGTH, MAX_PHONE_LENGTH, true);
            fprintf(stdout, "TODO: Get System Date.\n");
            
            do {
                printf("Enter opening balance (minimum $%.2f): $ ", MIN_OPENING_BALANCE);
                scanf("%lf", &opening_balance[client]);
                if (opening_balance[client] < MIN_OPENING_BALANCE) {
                    printf("Opening balance must be at least $%.2f.\n", MIN_OPENING_BALANCE);
                }
            } while (opening_balance[client] < MIN_OPENING_BALANCE);

            // Generate account number
            snprintf(account_number[client], sizeof(account_number[client]), "%s%u", rfc[client], client + 1);
            
            current_balance[client] = opening_balance[client];
            
            printf("\nClient registered successfully!\n\n");

            printf("Do you want to register another client? (y/n): $ ");
            do {
                scanf(" %c", &option);
                getchar(); // Clear the input buffer
                if (option != 'y' && option != 'n' && option != 'Y' && option != 'N')
                    printf("Error: Invalid option. Please, try again: $ ");
            } while (option != 'y' && option != 'n' && option != 'Y' && option != 'N');

            if (option == 'n' || option == 'N') {
                break; // Exit this submenu if the user doesn't want to register another client
            }
        }
    }

    // If there are no empty slots
    //printf("Error: No available slots to register a new client.\n");
    print_clients_and_info();
}

void remove_clients(void) {
    // FIXME: When all client slots are full and user tryes to remove one, the last slots aren't free.
    char option;
    s_string client_to_remove;
    printf("Enter the account number of the client to remove: $ ");
    scanf(" %s", client_to_remove);
    getchar(); // Clear the input buffer
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        if (strcmp(account_number[client], client_to_remove) == 0) {
            printf("Client %zu information:\n", client + 1);
            printf("Account number: %s\n", account_number[client]);
            printf("RFC: %s\n", rfc[client]);
            printf("Name: %s\n", name[client]);
            printf("Street: %s\n", street[client]);
            printf("Suburb: %s\n", suburb[client]);
            printf("City: %s\n", city[client]);
            printf("House number: %s\n", house_number[client]);
            printf("Phone number: %s\n", phone[client]);
            printf("Opening balance: $%.2f\n", opening_balance[client]);
            printf("Current balance: $%.2f\n", current_balance[client]);

            do {
                printf("Do you want to remove this client? (y/n): $ ");
                scanf(" %c", &option);
                getchar(); // Clear the input buffer
            } while (option != 'y' && option != 'n' && option != 'Y' && option != 'N');

            if (option == 'y' || option == 'Y') {
                printf("\nRearranging clients...\n");
                // Rearrange clients to fill the slot of the client that must be removed.
                for (size_t i = client; i < MAX_CLIENTS; i++) {
                    strcpy(rfc[i], rfc[i + 1]);
                    strcpy(name[i], name[i + 1]);
                    strcpy(street[i], street[i + 1]);
                    strcpy(suburb[i], suburb[i + 1]);
                    strcpy(city[i], city[i + 1]);
                    strcpy(house_number[i], house_number[i + 1]);
                    strcpy(phone[i], phone[i + 1]);
                    opening_balance[i] = opening_balance[i + 1];
                    current_balance[i] = current_balance[i + 1];
                    strcpy(account_number[i], account_number[i + 1]);
                }
                printf("Client removed successfully!\n");

            } else {
                printf("\nOperation aborted.\n");
            }
        }
    }
    print_clients_and_info();
}

void update_information(void) {
    s_string account;
    int account_index = (int);// <- get_account_index(ask_for_string("account number", account, ACCOUNT_NUMBER_LENGTH, ACCOUNT_NUMBER_LENGTH, true);
    int option;
    ask_for_string("account number", account, ACCOUNT_NUMBER_LENGTH, ACCOUNT_NUMBER_LENGTH, true);
    printf("Select what you want to change.\n");
    printf("0. Cancel.\n1. RFC.\n2.Name.\n3. Street.\n4. House number.\n5. Suburb.\n6. City.\n7. Phone number.\n");
    do {
        scanf(" %c", &option);
        if (option < 0 || option > 7)
            printf("Error: Invalid option. Please, try again: $ ");
    } while (option < 0 || option > 0);

    switch (option) {
        case 0: return;
        case 1:
            ask_for_string("RFC", rfc[account_index], MIN_RFC_LENGTH, MAX_RFC_LENGTH, false);
            break;
        default: break;
    }
}

void client_inquiry(void) {
    // Implementation here
}

void deposit(void) {
    // Implementation here
}

void withdraw(void) {
    // Implementation here
}

void check_balance(void) {
    // Implementation here
}

/// @brief Prompts the user for a string that meets certain requirements. Includes verification system.
/// @param item What will be requested from the user.
/// @param answer Where user's answer will be stored.
/// @param min_length The minimum length for the input.
/// @param max_length The minimum length for the input.
/// @param only_nums If true, user must enter numbers only; else, other characters are valid.
void ask_for_string(const char* item, char* answer, size_t min_length, size_t max_length, bool only_nums) {
    // '?' & ':' are ternary condition. If only_nums is true, it ads ", only numbers", else, ads nothin (an empty string).
    printf("Please, enter your %s (between %zu and %zu characters%s): $ ", item, min_length, max_length, only_nums ? ", only numbers" : "");
    do {
        /**
         * I used fgets() instead of scanf() 'cause it takes only as many characters as you want,
         * but (in this case) it takes max_length - 1, so that´s why I add 1 to max_length.
         * This method needs to clear the input buffer 'cause fgets() doesn´t do it by itself,
         * so the extra characters that user enters are passed automatically in the next call.
         */
        fgets(answer, max_length + 1, stdin);
        // If user enters an item with less than max characters, there will be a \n at the end, so let´s remove it.
        remove_newline(answer);
        // If answer has the max amount of characters and there isn't a \n, user entered more characters than allowed, so we need to clean the input buffer.
        if (strlen(answer) == max_length && answer[max_length - 1] != '\n') {
            int character;
            // Get characters in the buffer until it finds a \n or End Of File (EOF).
            while ((character = getchar()) != '\n' && character != EOF);
        }

        // Check if answer length is valid.
        if (strlen(answer) < min_length || strlen(answer) > max_length) {
            printf("Error: %s must have between %zu and %zu characters. Please, try again: $ ", item, min_length, max_length);
            continue; // If there's an error, repeat the loop.
        }

        // If only_nums is true, check if there is another character than nums.
        if (only_nums && strspn(answer, "0123456789") != strlen(answer)) {
            printf("Error: %s must contain only numbers. Please, try again: $ ", item);
            continue;
        }

        break; // All good, exit the loop.
    } while (1); // Doesn't finishes until a break statement is found.
}

/// @brief Removes \\n of a string.
/// @param str The string that have a \\n.
void remove_newline(char* str) {
    // str length - 1 is saved in length. We substract 1 'cause arrays starts from zero instead of 1.
    size_t length = strlen(str) - 1;
    // We look up if length have characters and if the last is a \n to replace it with nul (\0).
    if (length > 0 && str[length] == '\n')
        str[length] = '\0';
}

/// @brief Prints messages in a log file.
/// @param msg Message to print in the log file.
void print_log(const char *msg){
    FILE *file = fopen("bankProyect.log", "w");
    if (file == NULL) {
        perror("Error while opening log file");
        return;
    }

    fprintf(file, "%s", msg);
    fclose(file);
}

void print_clients_and_info(void) { // DEBUG //
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        printf("======================================\n");
        printf("%s\n", account_number[client]);
        printf("%s\n", rfc[client]);
        printf("%s\n", name[client]);
        printf("%s\n", street[client]);
        printf("%s\n", suburb[client]);
        printf("%s\n", city[client]);
        printf("%s\n", house_number[client]);
        printf("%s\n", phone[client]);
        printf("%hu\n", registration_day[client]);
        printf("%hu\n", registration_month[client]);
        printf("%u\n", registration_year[client]);
        printf("%.2f\n", current_balance[client]);
        printf("======================================\n");
    }
}