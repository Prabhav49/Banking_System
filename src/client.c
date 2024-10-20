#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "menu.h"
#include "customer.h"
#include "banking.h"
#include "manager.h"
#include "admin.h"
#include "employee.h"
#include <stdbool.h>
#include <stdint.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_INPUT_LENGTH 1024

int choice;

void sendLogoutStatus(int client_socket, bool loggedOut) {
    int statusToSend = loggedOut ? 1 : 0;

    // Send the integer value to the server
    if (write(client_socket, &statusToSend, sizeof(statusToSend)) < 0) {
        perror("Failed to send logout status to server");
    }
}

bool handleChoice(const char* username, int choice, const char* role) {
    // Implement role-specific choice handling here
    if (strcmp(role, "Customer") == 0) {
        customerCase(username, choice);
        return (choice == 11);
    } else if (strcmp(role, "Employee") == 0) {
        empCase(username, choice);
        return (choice == 8);
    } else if (strcmp(role, "Admin") == 0) {
        adminCase(username, choice);
        return (choice == 5);
    } else if (strcmp(role, "Manager") == 0) {
        manCase(username, choice);
        return (choice == 5);
    }
    return false; // Invalid role
}

void displayMenuBasedOnRole(const char* role) {
    char* menu = NULL;

    if (strcmp(role, "Customer") == 0) {
        menu = getCustomerMenu();
    } else if (strcmp(role, "Manager") == 0) {
        menu = getManagerMenu();
    } else if (strcmp(role, "Employee") == 0) {
        menu = getEmployeeMenu();
    } else if (strcmp(role, "Admin") == 0) {
        menu = getAdminMenu();
    } else {
        printf("Invalid role: %s\n", role);
        return;
    }
    printf("%s", menu);
    free(menu);
}

int send_string(int client_socket, const char *str) {
    uint32_t str_len = strlen(str);

    if (write(client_socket, &str_len, sizeof(str_len)) <= 0) {
        return -1;
    }
    if (write(client_socket, str, str_len) <= 0) {
        return -1;
    }
    return 0;
}

void flushInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {} 
}

void login_prompt(int client_socket) {
    while (1) {
        char username[MAX_INPUT_LENGTH];
        char password[MAX_INPUT_LENGTH];
        char role[MAX_INPUT_LENGTH];    

        printf("================ WELCOME TO BANK ================\n");

        // Input username from client side
        memset(username, 0, sizeof(username));
        printf("Enter Username: ");
        if (fgets(username, sizeof(username), stdin) == NULL) {
            printf("Error reading username.\n");
            continue;
        }
        username[strcspn(username, "\n")] = 0; // Remove newline

        // Send username
        if (send_string(client_socket, username) < 0) {
            printf("Failed to send username.\n");
            return;
        }

        // Input password from client side
        memset(password, 0, sizeof(password));
        printf("Enter Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL) {
            printf("Error reading password.\n");
            continue;
        }
        password[strcspn(password, "\n")] = 0; // Remove newline

        // Send password
        if (send_string(client_socket, password) < 0) {
            printf("Failed to send password.\n");
            return;
        }


        printf("===============================================\n");

        // Receive authentication status (1 for success, 0 for failure)
        int auth_status;
        int bytes_read = read(client_socket, &auth_status, sizeof(auth_status));
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        if (auth_status == 1) {
            // Authentication successful, now receive the role
            memset(role, 0, sizeof(role));
            bytes_read = read(client_socket, role, sizeof(role) - 1);
            if (bytes_read <= 0) {
                printf("Server disconnected while sending role.\n");
                break;
            }
            role[bytes_read] = '\0';  
            printf("Role: %s\n", role);
            do {
                displayMenuBasedOnRole(role);
                do {
                    printf("Enter Your Choice (Press 0 for Exit): ");
                    if (scanf("%d", &choice) != 1) {
                        flushInputBuffer(); // Clear invalid input
                        printf("Invalid input. Try again.\n");
                        continue;
                    }
                    flushInputBuffer(); // Clear buffer after reading choice
                    if (choice == 0) {
                        logout(username);
                        sendLogoutStatus(client_socket, true);
                        close(client_socket);
                        return;
                    }

                    // Validate choice based on role
                    if ((strcmp(role, "Customer") == 0 && (choice < 1 || choice > 12)) ||
                        (strcmp(role, "Employee") == 0 && (choice < 1 || choice > 9)) ||
                        (strcmp(role, "Manager") == 0 && (choice < 1 || choice > 6)) ||
                        (strcmp(role, "Admin") == 0 && (choice < 1 || choice > 5))) {
                        printf("Invalid Choice. Try Again..\n");
                        continue;
                    }
                    break;
                } while (1);

                if (handleChoice(username, choice, role)) {
                    break; 
                }
            } while (1);
            sendLogoutStatus(client_socket, true);
        } else {
            // Authentication failed
            printf("Login Unsuccessful!!\n");
            continue;
        }
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to the server failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to the server.\n");

    // Handle login prompt
    login_prompt(client_socket);

    close(client_socket);
    return 0;
}
