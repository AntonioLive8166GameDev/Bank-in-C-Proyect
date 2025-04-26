#include <stdarg.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTS 100
// User constants
#define MIN_PSWRD_LENGTH 8
#define MAX_PSWRD_LENGTH 30
#define ACCOUNT_NUMBER_LENGTH 10
#define MIN_NAME_LENGTH 2
#define MAX_NAME_LENGTH 50
#define MIN_RFC_LENGTH 12
#define MAX_RFC_LENGTH 13
#define MIN_STREET_LENGTH 2
#define MAX_STREET_LENGTH 30
#define MIN_HOUSE_NUMBER_LENGTH 2
#define MAX_HOUSE_NUMBER_LENGTH 10
#define MIN_SUBURB_LENGTH 2
#define MAX_SUBURB_LENGTH 20
#define MIN_CITY_LENGTH 2
#define MAX_CITY_LENGTH 20
#define MIN_PHONE_LENGTH 10
#define MAX_PHONE_LENGTH 13
#define MIN_OPENING_BALANCE 5000.00
#define MIN_BALANCE 3000.00
// Log constant
#define MAX_OPERATION_LOGS 100

typedef char s_string[17]; // A short string for RFC, phone number and other small size data.
typedef char string[50]; // A string for names, surnames and medium size data.

// Clients information. (arr[n + 1] 'cause it needs a \0 at the end, else it may cause unexpected
// behavior whit some functions). It would be beter with structs...
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
float opening_balance[MAX_CLIENTS + 1];
float current_balance[MAX_CLIENTS + 1];
string password[MAX_CLIENTS + 1];
s_string status[MAX_CLIENTS + 1];

//                                                                       _________________________
// Operations log.                                                      |client|log0|log1|log2|...|.
char operation[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Format:         |  c0  | op | op | op |...|.
unsigned short op_day[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same     |  c1  |day |day |day |...|.
unsigned short op_month[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same   |  c2  |mon.|mon.|mon.|...|.
unsigned int op_year[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same      |  c3  |year|year|year|...|.
unsigned short op_hour[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same    |  c4  |hour|hour|hour|...|.
unsigned short op_minute[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same  |  c5  |min.|min.|min.|...|.
unsigned short op_second[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // Same  |  c6  |sec.|sec.|sec.|...|.
float op_amount[MAX_CLIENTS + 1][MAX_OPERATION_LOGS]; // same           |  c7  | $$ | $$ | $$ |...|. 

// Submenus

void administrator_menu(void);
void client_menu(int account_index);

// Administrator options

void register_clients(void);
void remove_clients(void);
void update_information(void);
void client_inquiry(void);

// Client options

void deposit(int account_index);
void withdraw(int account_index);
void check_balance(int account_index);

// Tools

void ask_for_string(const char *item, char *answer, size_t min_length, size_t max_length,
        bool only_nums);
void remove_newline(char *str);
int get_account_index(const char *account);
int look_for_duplications(unsigned int client_index);
int get_time(int *store, char date_element);
void log_operation(unsigned int client_index, const char operation_key, float amount);

// Debug

void push_log(int line, const char *func, const char *label, const char *msg, ...);
void serialize_clients_data(void);
void serialize_clients_logs(void);
void print_clients_and_info(void);


void _ready(void) {
    push_log(__LINE__, __func__, "INFO", "Execution started. [START]");
    push_log(__LINE__, __func__, "DEBUG", "MAX_CLIENTS initialized to %d.", MAX_CLIENTS);
    // Initialize all clients account number to empty strings (to allow checking for empty clients).
    for (size_t client = 0; client == MAX_CLIENTS; client++) {
        strcpy(account_number[client], "");
        // Initialize all logs to \0 to allow checking for available log spaces.
        for (size_t log = 0; log < MAX_OPERATION_LOGS; log++) {
            operation[client][log] = '\0';
        }
    }
    strcpy(account_number[0], "0123456789\0");
    strcpy(rfc[0], "HEAJ061203J4\0");
    strcpy(name[0], "Antonio\0");
    strcpy(street[0], "mamawebo 190\0");
    strcpy(suburb[0], "Rio colorao\0");
    strcpy(city[0], "Deoyork\0");
    strcpy(house_number[0], "123\0");
    strcpy(phone[0], "3481655796\0");
    registration_day[0] = 03;
    registration_month[0] = 01;
    registration_year[0] = 2025;
    opening_balance[0] = 5000.00;
    current_balance[0] = 6700.00;
    strcpy(password[0], "juasjuas");
    strcpy(status[0], "ACTIVE\0");
    push_log(__LINE__, __func__, "DEBUG", "Client with account %s data initialized.",
            account_number[0]);

    strcpy(account_number[1], "1234567890\0");
    strcpy(rfc[1], "HEAJ061203J4\0");
    strcpy(name[1], "Antonio\0");
    strcpy(street[1], "mamawebo 190\0");
    strcpy(suburb[1], "Rio colorao\0");
    strcpy(city[1], "Deoyork\0");
    strcpy(house_number[1], "123\0");
    strcpy(phone[1], "3481655796\0");
    registration_day[1] = 03;
    registration_month[1] = 01;
    registration_year[1] = 2025;
    opening_balance[1] = 5000.00;
    current_balance[1] = 6700.00;
    strcpy(status[1], "ACTIVE\0");
    push_log(__LINE__, __func__, "DEBUG", "Client with account %s data initialized.",
            account_number[1]);

    serialize_clients_data();
}

int main(void) {
    _ready();
    char login_menu;
    string admin = "AntonioLive8166", admin_pswrd = "ArsevusBestDevs", attempt;

    do {
        printf("How do you want to sign in?\n0. Exit.\n1. Administrator.\n2. Client.\n\n$ ");
        scanf(" %c", &login_menu);
        getchar(); // Clear the input buffer.
        
        system("cls"); // Clear the console.
        switch (login_menu) {
            case '0': // Exit.
                printf("Program finished.\n");
                push_log(__LINE__, __func__, "INFO", "Program finished safely. [END]");
                break;
            
            case '1': // Administrator.
                do {
                    // Admin login.
                    printf("Welcome, %s. Enter your password: $ ", admin);
                    scanf(" %s", attempt);
                    system("cls"); // Clear the console.

                    if (strcmp(admin_pswrd, attempt) == 0) {
                        administrator_menu();
                    } else {
                        printf("Wrong password. Please, try again.\n");
                        push_log(__LINE__, __func__, "USER_ERROR", "Failed attempt to login.");
                    }

                } while (strcmp(admin_pswrd, attempt) != 0);
                break;

            case '2': // Client.
                s_string account;
                int account_index;

                // Asks for an account number and gets it's index.
                do {
                    ask_for_string("account number", account, ACCOUNT_NUMBER_LENGTH,
                            ACCOUNT_NUMBER_LENGTH, true);
                    account_index = get_account_index(account);
                } while (account_index < 0);

                /// FIXME: Password verification system for clients.
                // Clients login.
                // do {
                //     printf("%s", password[account_index]);
                //     printf("Welcome, %s. Enter your password: $ ", name[account_index]);
                //     scanf(" %s", attempt);

                //     if (strcmp(password[account_index], attempt) == 0) {
                //         client_menu(account_index);
                //     } else {
                //         printf("Wrong password. Please, try again.\n");
                //         push_log(__LINE__, __func__, "USER_ERROR", "Failed attempt.");
                //     }

                // } while (strcmp(password[account_index], attempt) != 0);
                // break;

                client_menu(account_index);
                break;

            default:
                printf("Invalid option. Please try again.\n");
                push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                break;
        }

    } while (login_menu != '0');
    
    return 0;
}

void administrator_menu(void) {
    push_log(__LINE__, __func__, "INFO", "Logged in as \"administrator\".");
    system("cls"); // Clear the console.

    char admin_menu;
    do {
        printf("Administrator Menu:\n0. Log out.\n1. Register clients.\n2. Remove clients.\n"
                "3. Update information.\n4. Client inquiry.\n\n$ ");
        scanf(" %c", &admin_menu);
        getchar();

        system("cls"); // Clear the console.
        switch (admin_menu) {
        case '0': // Log out.
            push_log(__LINE__, __func__, "INFO", "Logged out as \"administrator\".");
            break;
        
        case '1': // Register clients.
            register_clients();
            break;

        case '2': // Remove clients.
            remove_clients();
            break;

        case '3': // Update information.
            update_information();
            break;

        case '4': // Client inquiry.
            client_inquiry();
            break;

        default:
            printf("Invalid option. Please try again.\n");
            push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
            break;
        }

    } while (admin_menu != '0');
}

void client_menu(int account_index) {
    push_log(__LINE__, __func__, "INFO", "Logged in as \"client\" with account %s.",
            account_number[account_index]);
    char client_menu;
    do {
        printf("Client Menu:\n0. Log out.\n1. Deposit.\n2. Withdraw.\n"
                "3. Check balance.\n\n$ ");
        scanf(" %c", &client_menu);
        getchar(); // Clear the input buffer.

        system("cls"); // Clear the console.
        switch (client_menu) {
            case '0': // Log out.
                push_log(__LINE__, __func__, "INFO", "Client %s logged out.",
                        account_number[account_index]);
                break;
            
            case '1': // Deposit.
                deposit(account_index);
                break;

            case '2': // Withdraw.
                withdraw(account_index);
                break;

            case '3': // Check balance.
                check_balance(account_index);
                break;

            default:
                printf("Invalid option. Please try again.\n");
                push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                break;
        }

    } while (client_menu != '0');
}


/* ADMIN MENU */

/// @brief Looks for an empty slot in the built-in database and fills it with client's data.
void register_clients(void) {
    char option;
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        // Check if the client slot is empty to avoid overwriting existing clients.
        if (strcmp(account_number[client], "") == 0) {
            // Ask for RFC, name, street, suburb, city, house number and phone.
            printf("Registering client %zu:\n", client + 1);
            ask_for_string("RFC", rfc[client], MIN_RFC_LENGTH, MAX_RFC_LENGTH, false);
            ask_for_string("name", name[client], MIN_NAME_LENGTH, MAX_NAME_LENGTH, false);
            ask_for_string("street name", street[client], MIN_STREET_LENGTH, MAX_STREET_LENGTH,
                    false);
            ask_for_string("suburb name", suburb[client], MIN_SUBURB_LENGTH, MAX_SUBURB_LENGTH,
                    false);
            ask_for_string("city name", city[client], MIN_CITY_LENGTH, MAX_CITY_LENGTH, false);
            ask_for_string("house number", house_number[client], MIN_HOUSE_NUMBER_LENGTH,
                    MAX_HOUSE_NUMBER_LENGTH, true);
            ask_for_string("phone number", phone[client], MIN_PHONE_LENGTH, MAX_PHONE_LENGTH, true);
            
            // Asking for an opening balance.
            printf("Enter opening balance (minimum $%.2f): $", MIN_OPENING_BALANCE);
            do {
                scanf("%f", &opening_balance[client]);
                system("cls"); // Clear the console.
                if (opening_balance[client] < MIN_OPENING_BALANCE) {
                    printf("Error: Opening balance must be at least $%.2f. Please, try again: $",
                        MIN_OPENING_BALANCE);
                        push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                    }
                    
                } while (opening_balance[client] < MIN_OPENING_BALANCE);
                
                // Generate account number until is not duplicated.
                do {
                    /** Randomizing seed.
                     * srand = seed randomize I think XD?
                     * Establishes the initial initialization value for the pseudo-elementary numbers
                     * generator (seed value, which is it's parameter) used by the function rand()
                     * We pass time(NULL) as parameter cause it returns a very long integer value
                     * (it actually returns a time_t object which is a numeric representation of the
                     * tm struct, but it represents the same information as a numeric variable, 
                     * so it can be used here. At least I think this is how it works).
                     * More info: https://learn.microsoft.com/es-es/cpp/c-runtime-library/reference/srand?view=msvc-170
                     */
                srand(time(NULL)); // Randomizes the seed.
                char nums[] = "0123456789"; // Nums that are eligible for the RNG.
                // Chooses one character of nums[] for every horizontal position of account_number.
                // RGN: Random-Generated Number from a RNG.
                for (size_t rgn = 0; rgn < ACCOUNT_NUMBER_LENGTH; rgn++) {
                    // Here is better strlen than sizeof, 'cause strlen doesn't include the \0.
                    account_number[client][rgn] = nums[rand() % strlen(nums)];
                }
                account_number[client][ACCOUNT_NUMBER_LENGTH] = '\0'; // Add null terminator.
                
                // Keep going till look_for_duplications returns 0 (no duplications found).
            } while (look_for_duplications(client) != 0); 
            
            // Setting up current balance and status
            current_balance[client] = opening_balance[client];
            strcpy(status[client], "ACTIVE\0");
            
            /** Setting up registration date. 
             * I made a (cast) getting registration day & month because of it's types (unsigned
             * short). This is safe 'cause returned values don't exceed unsigned short capacity.
            */
            get_time((int*)&registration_day[client], 'd');
            get_time((int*)&registration_month[client], 'm');
            get_time(&registration_year[client], 'y');
            
            push_log(__LINE__, __func__, "INFO", "Client registered.");
            printf("Client registered successfully!\n\n");
            serialize_clients_data();
            
            printf("Do you want to register another client? (y/n): $ ");
            do {
                scanf(" %c", &option);
                getchar(); // Clear the input buffer.
                system("cls"); // Clear the console.
                if (option != 'y' && option != 'n' && option != 'Y' && option != 'N') {
                    printf("Error: Invalid option. Please, try again: $ ");
                    push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                }
            } while (option != 'y' && option != 'n' && option != 'Y' && option != 'N');

            if (option == 'n' || option == 'N') {
                return; // Exit this submenu if the user doesn't want to register another client.
            }
        }
    }

    system("cls"); // Clear the console.
    // If there are no available slots.
    push_log(__LINE__, __func__, "FAIL", "Ran out of slots to register new clients.");
    printf("Error: No available slots to register a new client.\n");
}

/// @brief Removes a client from the built-in database.
void remove_clients(void) {
    // Due to the client elimination and registration systems, if client0 has no account number 
    // means that there's no clients registered.
    if (strcmp(account_number[0], "") == 0) {
        push_log(__LINE__, __func__, "ERROR", "No registered users.");
        printf("Error: There are no registered users.\n\n");
        return;
    }

    char option;
    s_string client_to_remove; // The account number typed by user.
    /** Changed in v2.0.0: In v1.x.x this was inside of a for loop that was responsible for searching
     * the right account index. But since v2.0.0 I added a get_account_index function that does that
     * work better. Because I used this variable everywhere inside thad loop body, I decided not to 
     * change it.
     */
    int client; // The index of the client to remove

    // Get client account index.
    do {
        ask_for_string("account number", client_to_remove, ACCOUNT_NUMBER_LENGTH, ACCOUNT_NUMBER_LENGTH, true);
        client = get_account_index(client_to_remove);
    } while (client < 0);
    // Print client's information.
    printf("Client %zu information:\n\n", client + 1);
    printf("Account number: %s\n", account_number[client]);
    printf("RFC: %s\n", rfc[client]);
    printf("Name: %s\n", name[client]);
    printf("Address: %s %s, %s, %s\n", house_number[client], street[client], suburb[client],
            city[client]);
    printf("Phone number: %s\n", phone[client]);
    printf("Registration date: %hd/%hd/%d\n", registration_month[client],
            registration_day[client], registration_year[client]);
    printf("Current balance: $%.2f\n", current_balance[client]);
    printf("Status: \"%s\"\n", status[client]);

    /// @brief Asks to change status if current is "ACTIVE" and has funds.
    if (current_balance[client] > 0) {
        push_log(__LINE__, __func__, "FAIL", "Client %s has funds in his account.",
                account_number[client]);
        // Inform admin that client stills having funds.
        printf("\nWarning: There are funds in this account. Account balance must be zero to"
                " be able to delete.\n");
        
        // Inform admin about status and asks if want to switch it.
        if (strcmp(status[client], "ACTIVE") == 0){
            push_log(__LINE__, __func__, "FAIL", "Client %s status is \"ACTIVE\" but funds"
                    " remain unemptied.", account_number[client]);
            printf("Info: Client status is \"ACTIVE\", but must be \"PENDING_CLOSURE\" to"
                    " proceed.\n\nDo you want to change the status to \"PENDING_CLOSURE\"? "
                    "(y/n)\nWARNING: This action can NOT be undone. $ ");
            do {
                scanf(" %c", &option);
                getchar();
                system("cls");
                if (option != 'y' && option != 'n' && option != 'Y' && option != 'N') {
                    printf("Error: Invalid option. Please, try again: $ ");
                    push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                }
            } while (option != 'y' && option != 'n' && option != 'Y' && option != 'N');
            
            if (option == 'y' || option == 'Y') {
                strcpy(status[client], "PENDING_CLOSURE\0"); // Switch status.
                push_log(__LINE__, __func__, "INFO", "Client status changed from \"ACTIVE\""
                        " to \"%s\".", status[client]);
                serialize_clients_data();
                printf("Client status changed successfully! Now client is able to empty his"
                        " funds.\n\n");
            } else {
                push_log(__LINE__, __func__, "INFO", "Client status change aborted.");
                printf("Operation aborted.\n\n");
            }
        
        // If client has status "PENDING_CLOSURE" and funds.
        } else if (strcmp(status[client], "PENDING_CLOSURE") == 0) {
            push_log(__LINE__, __func__, "FAIL", "Client status is \"PENDING_CLOSURE\" but "
                    "funds remain unemptied.");
            printf("Info: Client status is \"PENDING_CLOSURE\", but his funds remain "
                    "unemptied. Come back after withdrawing all funds.\n\n");
        } else {
            // This shouldn't happen, but just in case (it happened once before xd).
            push_log(__LINE__, __func__, "CRITICAL", "Account %s status is null.",
                    account_number[client]);
            printf("WARNING! A critical error has ocurred: Client status is neither "
                    "\"ACTIVE\" nor \"PENDING_CLOSURE\".\n");
        }
        
    // If client don't have funds anymore and have status "PENDING_CLOSURE" can be removed.
    } else {
        printf("\nDo you want to remove this client? This can't be undone. (y/n): $ ");
        do {
            scanf(" %c", &option);
            getchar(); // Clear the input buffer
            system("cls"); // Clear the console.
            if (option != 'y' && option != 'n' && option != 'Y' && option != 'N') {
                printf("Error: Invalid option. Please, try again: $ ");
                push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
            }
        } while (option != 'y' && option != 'n' && option != 'Y' && option != 'N');

        if (option == 'y' || option == 'Y') {
            printf("Rearranging clients...\n");
            /** Rearrange clients.
             * Rearrange clients to fill the slot of the client that must be removed. This
             * method helps to avoid errors in the future and to keep data organized.
             */
            for (size_t i = client; i < MAX_CLIENTS; i++) {
                if (strcmp(account_number[i], "") == 0) {
                    break; // If this client is empty, is not necessary to continue.
                }
                strcpy(account_number[i], account_number[i + 1]);
                strcpy(rfc[i], rfc[i + 1]);
                strcpy(name[i], name[i + 1]);
                strcpy(street[i], street[i + 1]);
                strcpy(suburb[i], suburb[i + 1]);
                strcpy(city[i], city[i + 1]);
                strcpy(house_number[i], house_number[i + 1]);
                strcpy(phone[i], phone[i + 1]);
                registration_day[i] = registration_day[i + 1];
                registration_month[i] = registration_month[i + 1];
                registration_year[i] = registration_year[i + 1];
                opening_balance[i] = opening_balance[i + 1];
                current_balance[i] = current_balance[i + 1];
                strcpy(status[i], status[i + 1]);
                
                // Rearrange client's logs.
                for (size_t log = 0; log < MAX_OPERATION_LOGS; log++) {
                    if (operation[i][log] == '\0') {
                        break; // If current log is empty, is not necessary to continue.
                    }
                    operation[i][log] = operation[i + 1][log];
                    op_day[i][log] = op_day[i + 1][log];
                    op_month[i][log] = op_month[i + 1][log];
                    op_year[i][log] = op_year[i + 1][log];
                    op_hour[i][log] = op_hour[i + 1][log];
                    op_minute[i][log] = op_minute[i + 1][log];
                    op_second[i][log] = op_second[i + 1][log];
                    op_amount[i][log] = op_amount[i + 1][log];
                }
            }
            push_log(__LINE__, __func__, "INFO", "Client removed.");
            serialize_clients_data();
            serialize_clients_logs();
            printf("Client removed successfully!\n\n");

        } else {
            push_log(__LINE__, __func__, "INFO", "Client removing aborted.");
            printf("Operation aborted.\n\n");
        }
    }
}

/// @brief Updates client information in the built-in database. 
void update_information(void) {
     // Due to the client elimination and registration systems, if client0 has no account number
     // means that there's no clients registered.
     if (strcmp(account_number[0], "") == 0) {
        push_log(__LINE__, __func__, "ERROR", "No registered users.");
        printf("Error: There are no registered users.\n\n");
        return;
    }

    s_string account;
    int account_index;
    char u_i_option;
    do {
        // Asks for an account number and gets it's index.
        do {
            ask_for_string("account number", account, ACCOUNT_NUMBER_LENGTH, ACCOUNT_NUMBER_LENGTH,
                    true);
            account_index = get_account_index(account);
        } while (account_index < 0); // -1 Means nonexistent account.
        char option;
        do {
            printf("Select what you want to change.\n");
            printf("0. Back.\n1. RFC.\n2. Name.\n3. Street.\n4. House number.\n5. Suburb.\n6. City."
                    "\n7. Phone number.\n\n$ ");
            scanf(" %c", &option);
            getchar();
            
            system("cls"); // Clear the console.
            switch (option) {
                case '0': break; // Back.

                case '1': // RFC.
                    ask_for_string("new RFC", rfc[account_index], MIN_RFC_LENGTH, MAX_RFC_LENGTH,
                            false);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"rfc\" of client %d.",
                            account_index);
                    printf("Atribute \"rfc\" changed successfully!\n\n");
                    break;
                
                case '2': // Name.
                    ask_for_string("new name", name[account_index], MIN_NAME_LENGTH,
                            MAX_NAME_LENGTH, false);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"name\" of client %d.",
                            account_index);
                    printf("Atribute \"name\" changed successfully!\n\n");
                    break;
                
                case '3': // Street.
                    ask_for_string("new street", street[account_index], MIN_STREET_LENGTH,
                            MAX_STREET_LENGTH, false);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"street\" "
                            "of client %d.", account_index);
                    printf("Atribute \"street\" changed successfully!\n\n");
                    break;
                
                case '4': // House number.
                    ask_for_string("new house number", house_number[account_index],
                            MIN_HOUSE_NUMBER_LENGTH, MAX_HOUSE_NUMBER_LENGTH, true);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"house_number\" of "
                            "client %d.", account_index);
                    printf("Atribute \"house number\" changed successfully!\n\n");
                    break;
                
                case '5': // Suburb.
                    ask_for_string("new suburb", suburb[account_index], MIN_SUBURB_LENGTH,
                            MAX_SUBURB_LENGTH, false);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"suburb\" "
                            "of client %d.", account_index);
                    printf("Atribute \"suburb\" changed successfully!\n\n");
                    break;
                    
                case '6': // City.
                    ask_for_string("new city", city[account_index], MIN_CITY_LENGTH,
                            MAX_CITY_LENGTH, false);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"city\" of client %d.",
                            account_index);
                    printf("Atribute \"new city\" changed successfully!\n\n");
                    break;
                
                case '7': // Phone number
                    ask_for_string("new phone number", phone[account_index], MIN_PHONE_LENGTH,
                            MAX_PHONE_LENGTH, true);
                    push_log(__LINE__, __func__, "INFO", "Changed atribute \"phone\" of client %d.",
                            account_index);
                    printf("Atribute \"phone number\" changed successfully!\n\n");
                    break;

                default: 
                    printf("Error: Invalid option. Please, try again.\n");
                    push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                    break;
            }
            /**
             * strchr() (string character) retunrs a pointer to the FIRST time the character (2nd
             * arg) appears in the string (1st argument). Here this helps to avoid writing a lot of
             * || operators, making the code more readable.
             */
            if (strchr("1234567", option)) { // Check if option is a character in "1234567".
                serialize_clients_data();
            }

        } while (option != '0');

        system("cls"); // Clear the console.
        printf("Do you want to change another client's information? (y/n): $ ");
        do {
            scanf(" %c", &u_i_option);
            getchar();
            system("cls"); // Clear the console.
            if (u_i_option != 'y' && u_i_option != 'Y' && u_i_option != 'n' && u_i_option != 'N') {
                push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                printf("Error: Invalid option. Please, try again: $ ");
            }

        }while (u_i_option != 'y' && u_i_option != 'Y' && u_i_option != 'n' && u_i_option != 'N');

    } while (u_i_option != 'n' && u_i_option != 'N');

    system("cls"); // Clear the console.
}

/// @brief Asks for a client and prints it's info.
void client_inquiry(void) {
    // Due to the client elimination and registration systems, if client0 has no account number
    // means that there's no clients registered.
    if (strcmp(account_number[0], "") == 0) {
        push_log(__LINE__, __func__, "ERROR", "No registered users.");
        printf("Error: There are no registered users.\n\n");
        return;
    }
    
    s_string account;
    int account_index;
    char option;
    do {
        // Asks for an account number and gets it's index.
        do {
            ask_for_string("account number", account, ACCOUNT_NUMBER_LENGTH, ACCOUNT_NUMBER_LENGTH,
                    true);
            account_index = get_account_index(account);
        } while (account_index < 0);
        
        printf("Client %d information:\n\n", account_index + 1);
        printf("Account number: %s\n", account_number[account_index]);
        printf("RFC: %s\n", rfc[account_index]);
        printf("Name: %s\n", name[account_index]);
        printf("Street: %s\n", street[account_index]);
        printf("Suburb: %s\n", suburb[account_index]);
        printf("City: %s\n", city[account_index]);
        printf("House number: %s\n", house_number[account_index]);
        printf("Phone number: %s\n", phone[account_index]);
        printf("Registration Day: %hu\n", registration_day[account_index]);
        printf("Registration Month: %hu\n", registration_month[account_index]);
        printf("Registration Year: %u\n", registration_year[account_index]);
        printf("Opening Balance: %.2f\n", opening_balance[account_index]);
        printf("Current Balance: %.2f\n", current_balance[account_index]);

        push_log(__LINE__, __func__, "INFO", "Printed data of client %d.", account_index);

        printf("\nDo you want to see another client's information? (y/n): $ ");
        do {
            scanf(" %c", &option);
            getchar();
            system("cls"); // Clear the console.
            if (option != 'y' && option != 'Y' && option != 'n' && option != 'N') {
                push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
                printf("Error: Invalid option. Please, try again: $ ");
            }

        }while (option != 'y' && option != 'Y' && option != 'n' && option != 'N');

    } while (option != 'n' && option != 'N');

    system("cls"); // Clear the console.
}


/* CLIENT MENU */

/// @brief Deposits an amount to the specified account. Current balance mustn´t be less than 3000.00
/// @param account_index The index of the account to deposit to.
void deposit(int account_index) {
    // If client's status is "PENDING_CLOSURE", can't deposit.
    if (strcmp(status[account_index], "PENDING_CLOSURE") == 0) {
        push_log(__LINE__, __func__, "FAIL", "Client %s is \"PENDING_CLOSURE\" and can't deposit.",
                account_number[account_index]);
        printf("Error: Your account has status \"PENDING_CLOSURE\" and can't deposit.\n\n");
        return;
    }

    float deposit_amount;
    printf("Enter the amount to deposit: $");
    do {
        scanf("%f", &deposit_amount);
        system("cls"); // Clear the console.
        if (deposit_amount <= 0) {
            printf("Error: Deposit amount must be greater than zero. Please, try again: $");
            push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
        }
    } while (deposit_amount <= 0);
    
    current_balance[account_index] += deposit_amount; // Add the deposit to current balance.
    push_log(__LINE__, __func__, "INFO", "Deposit of $%f made to account %s. Current balance: $%f.",
            deposit_amount, account_number[account_index], current_balance[account_index]);
    log_operation(account_index, 'd', deposit_amount); // Save the info of the transaction.
    printf("Deposit successful! New balance: $%.2f\n\n", current_balance[account_index]);
    serialize_clients_data();
}

/// @brief Withdraws an amount from the specified account. Current balance mustn´t be less than 3000.00
/// @param account_index The index of the account to withdraw from.
void withdraw(int account_index) {
    float withdraw_amount;
    printf("Enter the amount to withdraw: $");
    do {
        scanf("%f", &withdraw_amount);
        system("cls"); // Clear the console.
        if (withdraw_amount <= 0) {
            printf("Error: Withdraw amount must be greater than zero. Please, try again.");
            push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");

        // This condition may change according to client's status.
        } else if (strcmp(status[account_index], "ACTIVE") == 0 ?
                // ACTIVE: Minimum balance must be MIN_BALANCE.
                (current_balance[account_index] - withdraw_amount < MIN_BALANCE) :
                // Not ACTIVE (PENDING_CLOSURE): Minimum balance must be 0.
                (current_balance[account_index] - withdraw_amount < 0)) {

            // I couldn't do this with ternary condition :(.
            if (strcmp(status[account_index], "ACTIVE") == 0) {
                printf("Error: Insufficient funds. Minimum balance must be $%.2f. Please, "
                        "try again: $", MIN_BALANCE);
            } else {
                printf("Error: Insufficient funds. Minimum balance must be $0.00. Please, "
                        "try again: $");
            }

            push_log(__LINE__, __func__, "FAIL", "Insufficient funds.");
        }

    } while (withdraw_amount <= 0 || (strcmp(status[account_index], "ACTIVE") == 0 ?
            // ACTIVE: Minimum balance must be MIN_BALANCE.
            current_balance[account_index] - withdraw_amount < MIN_BALANCE :
            // Not ACTIVE (PENDING_CLOSURE): Minimum balance must be 0.
            current_balance[account_index] - withdraw_amount < 0));

    current_balance[account_index] -= withdraw_amount; // Substract amount from current balance.
    push_log(__LINE__, __func__, "INFO", "Withdrawal of $%f made from account %s. Current balance: "
            "$%f.", withdraw_amount, account_number[account_index], current_balance[account_index]);
    log_operation(account_index, 'w', withdraw_amount); // Save the info of the transactioN.
    printf("Withdrawal successful! New balance: $%.2f\n\n", current_balance[account_index]);
    serialize_clients_data();
}

/// @brief Looks up to the current balance of the specified account.
/// @param account_index The index of the account to check the balance of.
void check_balance(int account_index) {
    system("cls"); // Clear the console.
    time_t current_time = time(NULL);
    string date;
    strftime(date, sizeof(date), "%m/%d/%Y %H:%M:%S", localtime(&current_time));
    printf("Date & time: %s\n", date);
    printf("Account number: %s\n", account_number[account_index]);
    printf("Name: %s\n", name[account_index]);
    printf("Current balance: $%.2f\n\n", current_balance[account_index]);
    push_log(__LINE__, __func__, "INFO", "Checked balance of account %s. Current balance: $%f.",
            account_number[account_index], current_balance[account_index]);
}


/* TOOLS */

/// @brief Prompts the user for a string that meets certain requirements. Includes verification system.
/// @param item What will be requested from the user.
/// @param answer Where user's answer will be stored.
/// @param min_length The minimum length for the input.
/// @param max_length The minimum length for the input.
/// @param only_nums If true, user must enter numbers only; else, other characters are valid.
void ask_for_string(const char* item, char* answer, size_t min_length, size_t max_length,
        bool only_nums) {
    // '?' & ':' are ternary condition. If only_nums is true, it ads ", only numbers", else, ads
    // nothing (an empty string). OMG this is so helpful, I loved it :D.
    printf("Please, enter the %s (between %zu and %zu characters%s): $ ", item, min_length,
            max_length, only_nums ? ", only numbers" : "");
    do {
        /**
         * I used fgets() instead of scanf() 'cause it takes only as many characters as you want,
         * but (in this case) it takes max_length - 1, so that´s why I add 1 to max_length.
         * This method needs to clear the input buffer 'cause fgets() doesn´t do it by itself,
         * so the extra characters that user enters are passed automatically in the next call.
         */
        fgets(answer, max_length + 1, stdin);
        // Adding the \0 character at the end (to avoid unexpected behavior with some functions).
        answer[max_length] = '\0';
        // If user enters an item with less than max characters, there will be a \n at the end, so
        // let's remove it.
        remove_newline(answer);

        // If answer has the max amount of characters and there isn't a \n, user entered more
        // characters than allowed, so we need to clean the input buffer.
        if (strlen(answer) == max_length && answer[max_length - 1] != '\n') {
            int character;
            // Get characters in the buffer until it finds a \n or End Of File (EOF).
            while ((character = getchar()) != '\n' && character != EOF);
        }

        system("cls"); // Clear the console.
        // Check if answer length is valid.
        if (strlen(answer) < min_length || strlen(answer) > max_length) {
            printf("Error: %s must have between %zu and %zu characters. Please, try again: $ ",
                    item, min_length, max_length);
            push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
            continue; // If there's an error, repeat the loop.
        }

        /** If only_nums is true, check if there is another character than nums.
         * strspn() (string span) returns how many characters of the string in the second argument
         * appears in the first string (first argument) until another character is found.
         * In this scenario, if the returned value of strspn doesn't matches the length of the
         * answer (ignoring the \0), means that the string contains non numeric characters.
        */
        if (only_nums && strspn(answer, "0123456789") != strlen(answer)) {
            printf("Error: %s must contain only numbers. Please, try again: $ ", item);
            push_log(__LINE__, __func__, "USER_ERROR", "Invalid input.");
            continue; // If there's an error, repeat the loop.
        }

        break; // When all is good, exit the loop.
    } while (1); // Doesn't finishes until a break statement is found.
}

/// @brief Removes \\n at the end of a string.
/// @param str The string that have a \\n.
void remove_newline(char* str) {
    // str length - 1 is saved in length. We substract 1 'cause arrays starts from zero
    // instead of 1.
    size_t length = strlen(str) - 1;
    // We look up if length have characters and if the last is a \n to replace it with null (\0).
    if (length > 0 && str[length] == '\n')
        str[length] = '\0';
}

/// @brief Check if an account exists and returns it's array index, else, logs an error.
/// @param account The account number that will be searched.
/// @return The index in the arrays of the account received.
///         -1: Nonexistent account.
int get_account_index(const char *account) {
    for (unsigned int index = 0; index < MAX_CLIENTS; index++) {
        // Check if the account number of the current index matches with the account received.
        if (strcmp(account_number[index], account) == 0)
            return index; // If true, returns the current account index.
    }

    // If none matches, returns the error code -1.
    printf("Error: Nonexistent account %s.\n", account);
    push_log(__LINE__, __func__, "FAIL", "Nonexistent account %s.", account);
    return -1;
}

/// @brief Checks if the new account number already exists.
/// @param client_index The index of the new account.
/// @return -1: Error: Account number already exists.   
///          0: No duplications found. Account number is secure to use.
int look_for_duplications(unsigned int client_index) {
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        // Check if the current account matches witch another (ignoring the account received).
        if (strcmp(account_number[client_index], account_number[client]) == 0 && client_index !=
                client){
            push_log(__LINE__, __func__, "ERROR", "Account number already exists.");
            return -1; // If true, returns the error code -1.
        }
    }

    // If none matches, returns 0, that means there's no accounts with same account number.
    push_log(__LINE__, __func__, "INFO", "No duplications found.");
    return 0;
}

/// @brief Gets the selected date element (day, month or year).
/// @param store A pointer to the variable which will store the result. If 0, returns by mame only. 
/// @param date_element 'd': Day, 'm': Month 'y': Year, 'H': Hour, 'M': Minute, 'S': Second.
/// @return By name and by interface: The selected date element converted to int.
/// @details This function returns by name and by interface 'cause I wanted to experiment with it.
int get_time(int *store, char date_element) {
    /** time(), strftime() & localtime().
     * time() returns the seconds since January 1, 1970 in a time_t object. We pass NULL
     * as argument 'cause it returns by name and by interface, but if we pass NULL,
     * only returns by name (we need it 'cause we are assigning the function call to a
     * variable. It could be maked also in this way (by interface):
     * @example
     * time_t current_time;
     * time(&current_time);
     * 
     * strftime() (string format time) formats a date and time and converts it to a string.
     * The format is analogous to that of the printf() family of functions, but the %
     * modifiers in this case are used to format dates.
     * @example
     * time_t current_time = time(NULL);
     * char datef[50]; // datef = date formated.
     * strftime(datef, sizeof(datef), "%d/%m/%Y %H:%M:%S", localtime(&current_time));
     * printf("Current time: %s\n", datef);
     * 
     * @example Output of the code above: Current time: 24/04/2025 18:32:05
     * 
     * localtime() converts a time value and corrects it for the local time zone. There
     * are more important details, but I'm not writing more. It's fun, but I'm tired.
     * More info: https://algoritmos9511.gitlab.io/_downloads/e1ac04d57c11925f0283040c533417bb/tiempo.pdf
     * More info: https://www.geeksforgeeks.org/time-function-in-c/
     * More info: https://learn.microsoft.com/es-es/cpp/c-runtime-library/reference/localtime-localtime32-localtime64?view=msvc-170
     * 
     * Pd: Returning by interface means modifying what a pointer supplied as an argument points to;
     * while returning by name refers to using the return statement explicitly.
     */            
    time_t current_time = time(NULL);
    s_string _date;
    if (date_element == 'd') {
        strftime(_date, sizeof(_date), "%d", localtime(&current_time));
    } else if (date_element == 'm') {
        strftime(_date, sizeof(_date), "%m", localtime(&current_time));
    } else if (date_element == 'y') {
        strftime(_date, sizeof(_date), "%Y", localtime(&current_time));
    } else if (date_element == 'H') {
        strftime(_date, sizeof(_date), "%H", localtime(&current_time));
    } else if (date_element == 'M') {
        strftime(_date, sizeof(_date), "%M", localtime(&current_time));
    } else if (date_element == 'S') {
        strftime(_date, sizeof(_date), "%Y", localtime(&current_time));
    } else {
        push_log(__LINE__, __func__, "ERROR", "Invalid argument 1: Expected 'd', 'm', 'y', 'H', 'M'"
                " or 'Y'; but received '%c'.", date_element);
        return -1;
    }
    
    if (store == 0) {
        return atoi(_date); // atoi = ASCII to integer.
    }

    *store = atoi(_date);
    return 0;
}

/// @brief Logs the transaction and serializes it in a JSON file.
/// @param client_index Index of the client who made the transaction.
/// @param operation_key d: Deposit. w: Withdraw.
/// @param amount The amount of the transaction.
void log_operation(unsigned int client_index, const char operation_key, float amount) {
    for (size_t log = 0; log < MAX_OPERATION_LOGS; log++) {
        if (operation[client_index][log] == '\0') {
            operation[client_index][log] = operation_key;
            op_day[client_index][log] = get_time(0, 'd');
            op_month[client_index][log] = get_time(0, 'm');
            op_year[client_index][log] = get_time(0, 'y');
            op_hour[client_index][log] = get_time(0, 'H');
            op_minute[client_index][log] = get_time(0, 'M');
            op_second[client_index][log] = get_time(0, 'S');
            op_amount[client_index][log] = amount;
            break; // Without this, all log slots are filled.
        }
    }

    serialize_clients_logs();
}


/* DEBUG */

/// @brief Pushes custom errors, warnings, etc. in a log file.
/// @param line LINE standard int.
/// @param func func standard const char[].
/// @param label Label of the log (e.g. DEBUG, ERROR, WARNING, INFO, USER_ERROR, FAIL, FATAL).
/// @param msg The message that will be pushed in the log file.
void push_log(int line, const char *func, const char *label, const char *msg, ...){
    // Opening a file with append permisions. 
    FILE *file = fopen("bankProyect.log", "a");  // fopen = file open.
    if (file == NULL) {
        // Prints an error message in stderr (standard error) using the standard variable errno.
        perror("Error while opening log file"); // perror = print error.
        return;
    }
    /** Fixed part of the log
     * strrchr() (string reverse character) retunrs a pointer to the last time the character (2nd
     * arg) appears in the string (1st arg.). This is to avoid writing the entire path of the file.
     */
    fprintf(file, "• %s %s\t%s:%d @ %s(): %s: ", __DATE__, __TIME__, strrchr(__FILE__, '\\'), line,
            func, label); // fprintf = file print formated; printf = print formated.
    
    // Variable arguments (to support format).
    // The "container" that stores the variable arguments .
    va_list args; //va_list = variable arguments list.
    // Initialize the argument list, specifying the last fixed argument to the function (msg). This
    // tells the program where the variable arguments begin in memory.
    va_start(args, msg); // va_start = start variable arguments processing (variable args. start).
    vfprintf(file, msg, args); // Writes formated msg in a file. (variable file print formated).
    // End the variable argument processing and release resources. This is so important to avoid
    // loosing data.
    va_end(args); // va_end = end variable arguments processing (variable arguments end).

    fprintf(file, "\n"); // Add a newline at the end.
    // Close the file stream. Important to free resources and ensure data integrity.
    fclose(file); // fclose = file close.
}

/// @brief Serializes the clients data to a JSON file.
/// @details The JSON file is created in the same directory as the source code. 
/// The file is overwritten every time this function is called.
void serialize_clients_data(void){
    // Opening a file with write permissions.
    FILE *file = fopen("temp_clients_info.json", "w");
    if (file == NULL) {
        push_log(__LINE__, __func__, "ERROR", "Couldn't be able to open JSON file.");
        // Prints an error message in stderr using the standard variable errno.
        perror("Error while opening JSON file");
        return;
    }

    // "Serialize" data.
    fprintf(file, "{\n");
    fprintf(file, "\t\"clients\": {\n");
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        if (strcmp(account_number[client], "") == 0) {
            break; // If client is empty, finish.
        }
        fprintf(file, "\t\t\"client%zu\": {\n", client);
        fprintf(file, "\t\t\t\"accountNumber\": \"%s\",\n", account_number[client]);
        fprintf(file, "\t\t\t\"rfc\": \"%s\",\n", rfc[client]);
        fprintf(file, "\t\t\t\"name\": \"%s\",\n", name[client]);
        fprintf(file, "\t\t\t\"street\": \"%s\",\n", street[client]);
        fprintf(file, "\t\t\t\"suburb\": \"%s\",\n", suburb[client]);
        fprintf(file, "\t\t\t\"city\": \"%s\",\n", city[client]);
        fprintf(file, "\t\t\t\"houseNumber\": \"%s\",\n", house_number[client]);
        fprintf(file, "\t\t\t\"phone\": \"%s\",\n", phone[client]);
        fprintf(file, "\t\t\t\"registrationDay\": %hu,\n", registration_day[client]);
        fprintf(file, "\t\t\t\"registrationMonth\": %hu,\n", registration_month[client]);
        fprintf(file, "\t\t\t\"registrationYear\": %u,\n", registration_year[client]);
        fprintf(file, "\t\t\t\"openingBalance\": %f,\n", opening_balance[client]);
        fprintf(file, "\t\t\t\"currentBalance\": %f,\n", current_balance[client]);
        fprintf(file, "\t\t\t\"password\": \"%s\",\n", password[client]);
        fprintf(file, "\t\t\t\"status\": \"%s\"\n", status[client]);
        // The last objet can't be finished with a comma.
        fprintf(file, "\t\t}%s", strcmp(account_number[client + 1], "") == 0 ? "\n" : ",\n");
    }
    fprintf(file, "\t}\n");
    fprintf(file, "}");

    fclose(file);
    push_log(__LINE__, __func__, "INFO", "JSON serialized.");
    
    /** @example Result of one client:
     * {
     *     "clients": {
     *         "client0": {
     *             "accountNumber": "123456",
     *             "rfc": "HEAJ061203J4",
     *             "name": "Jesús Antonio Hernández Aceves",
     *             "street": "Without street name",
     *             "suburb": "Degollado",
     *             "city": "Without city name",
     *             "houseNumber": "00",
     *             "phone": "3481355796",
     *             "registrationDay": 15,
     *             "registrationMonth": 4,
     *             "registrationYear": 2025,
     *             "openingBalance": 8166.00,
     *             "currentBalance": 81668166.00,
     *             "password": "WhatIsLove_BabyDontHurtMe_DontHurtMe_NoMore",
     *             "status": "ACTIVE"
     *         }
     *     }
     * }
    */
}

/// @brief Serializes lients logs to a JSON file.
/// @details The JSON file is created in the same directory as the source code. 
/// The file is overwritten every time this function is called.
void serialize_clients_logs(void) {
    FILE *file = fopen("clients_operation_log.json", "w");
    if (file == NULL) {
        push_log(__LINE__, __func__, "ERROR", "Couldn't be able to open JSON file.");
        perror("Error while opening JSON file");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "\t\"clients logs\": {\n");
    // Clients.
    for (size_t client = 0; client < MAX_CLIENTS; client++) {
        if (operation[client][0] == '\0') {
            break; // Finish if client doesn't has any logs.
        }

        fprintf(file, "\t\t\"client%zu\": [\n", client);
        // Logs.
        for (size_t log = 0; log < MAX_OPERATION_LOGS; log++){
            if (operation[client][log] == '\0') {
                break; // Finish if log is empty.
            }
            fprintf(file, "\t\t\t{\n");
            fprintf(file, "\t\t\t\t\"operation\": \"%s\",\n", operation[client][log] == 'd' ? "deposit" :
                    "withdraw");
            fprintf(file, "\t\t\t\t\"timestamp\": \"%hu-%hu-%u %hu:%hu:%hu\",\n", op_month[client][log],
                    op_day[client][log], op_year[client][log], op_hour[client][log],
                    op_minute[client][log], op_second[client][log]);
            fprintf(file, "\t\t\t\t\"amount\": %.2f\n", op_amount[client][log]);
            // Last object can't be finished with a comma.
            fprintf(file, "\t\t\t}%s", operation[client][log + 1] == '\0' ? "\n" : ",\n");
        }
        // The last array can't be finished with a comma.
        fprintf(file, "\t\t]%s", operation[client + 1][0] == '\0' ? "\n" : ",\n");
    }
    fprintf(file, "\t}\n");
    fprintf(file, "}");

    fclose(file);
    push_log(__LINE__, __func__, "INFO", "JSON serialized.");
}
/// @brief Prints all clients and their information.
/// @deprecated Use serialize_clients_data() instead.
void print_clients_and_info(void) { 
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
        printf("%s\n", status[client]);
        printf("======================================\n");
    }
}
