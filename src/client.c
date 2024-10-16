#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "menu.h"
#include "customer.h"
#include "manager.h"
#include "admin.h"
<<<<<<< HEAD
#include "employee.h"
#include <stdbool.h>


#define PORT 8080
bool loggedOut;
int choice;


void sendLogoutStatus(int client_socket, bool loggedOut) {
    int statusToSend = loggedOut ? 1 : 0;
    
    // Send the integer value to the server
    if (write(client_socket, &statusToSend, sizeof(statusToSend)) < 0) {
        perror("Failed to send logout status to server");
    }
}

void handleChoice(const char*username, int choice, const char* role){

    if(strcmp(role,"Customer")==0){
        customerCase(username,choice);
        if(choice == 11 ) loggedOut = true;
    }
    else if(strcmp(role,"Employee")==0){
        empCase(username,choice);
        if(choice == 8 ) loggedOut = true;
    }
    else if(strcmp(role,"Admin")==0){
        adminCase(username,choice);
        if(choice == 5 ) loggedOut = true;
    }
    else if(strcmp(role,"Manager")==0){
        manCase(username,choice);
        if(choice == 5 ) loggedOut = true;
    }
    
}

void displayMenuBasedOnRole(const char* buffer) {
    if (strcmp(buffer, "Customer") == 0) {
        char* menu = getCustomerMenu();
        printf("%s", menu);  // Print the Customer menu
        free(menu);         // Free the allocated memory
    }
    else if (strcmp(buffer, "Manager") == 0) {
        char* menu = getManagerMenu();
        printf("%s", menu);  // Print the Manager menu
        free(menu);         // Free the allocated memory
    }
    else if (strcmp(buffer, "Employee") == 0) {
        char* menu = getEmployeeMenu();
        printf("%s", menu);  // Print the Employee menu
        free(menu);         // Free the allocated memory
    }
    else if (strcmp(buffer, "Admin") == 0) {
        char* menu = getAdminMenu();
        printf("%s", menu);  // Print the Admin menu
        free(menu);         // Free the allocated memory
    }
    else {
        printf("Invalid role: %s\n", buffer);
    }
}


void login_prompt(int client_socket) {
    while (1) {  // Infinite loop to prompt for login
        char username[1024];
        char password[1024];
        char buffer[1024];
        char role[1024];

        // Display and input username from client side
        printf("Enter Username: ");
        if (fgets(username, sizeof(username), stdin) == NULL) {
            printf("Error reading username.\n");
            continue;
        }
        username[strcspn(username, "\n")] = 0;  // Remove newline character

        // Send username to server
        write(client_socket, username, strlen(username));

        // Display and input password from client side
        printf("Enter Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL) {
            printf("Error reading password.\n");
            continue;
        }
        password[strcspn(password, "\n")] = 0;  // Remove newline character

        // Send password to server
        write(client_socket, password, strlen(password));

        // Receive and display the authentication message from the server
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break;  // Exit the loop if the server disconnects
        }
        buffer[bytes_read] = '\0';  // Properly null-terminate the string
        printf("%s\n", buffer);  // Print the authentication result from the server

        // Check if authentication was successful
        if (strcmp(buffer, "Login Successful!\n") == 0) {
            // Receive the role from the server
            memset(role, 0, sizeof(role));  // Clear the buffer
            bytes_read = read(client_socket, role, sizeof(role) - 1);
            if (bytes_read <= 0) {
                printf("Server disconnected while sending role.\n");
                break;
            }
            role[bytes_read] = '\0';  // Properly null-terminate the string

            // Print the role
            printf("Role: %s\n", role);
            while (!loggedOut) {
                displayMenuBasedOnRole(role);
                int choice;
                printf("Enter Your Choice : ");
                scanf("%d", &choice);
                while (getchar() != '\n'); // Clear input buffer to handle leftover newline
                handleChoice(username, choice, role);
            }
            sendLogoutStatus(client_socket, loggedOut);
        } else {
            printf("Login failed. Try again.\n");
        }
    }
}

=======
<<<<<<< HEAD

#define MAX_BUFFER 1024
=======
>>>>>>> eb84e4dd2cdbdde4094cb23157fe8cbad42d2cda
>>>>>>> 2e475b2db8a3364cdb5353b4c7ab08ce150de7cd

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket (IPv4, TCP)
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to the server failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to the server.\n");

    // Call login_prompt directly since we are not waiting for any server prompt
    login_prompt(client_socket);  // Handle username and password input

<<<<<<< HEAD
    close(client_socket);
=======
        // Input username
        scanf("%s", username);
        send(sockfd, username, sizeof(username), 0);

        // Receive password prompt from server
        memset(buffer, 0, sizeof(buffer));
        if (recv(sockfd, buffer, sizeof(buffer), 0) <= 0) {
            perror("Error receiving password prompt");
            close(sockfd);
            exit(0);
        }
        printf("%s", buffer);

        // Input password
        scanf("%s", password);
        send(sockfd, password, sizeof(password), 0);

        // Receive login response from server
        memset(buffer, 0, sizeof(buffer));
        if (recv(sockfd, buffer, sizeof(buffer), 0) <= 0) {
            perror("Error receiving login response");
            close(sockfd);
            exit(0);
        }
        printf("%s\n", buffer);

        // Check if login is successful
        if (strcmp(buffer, "Login Successful!") == 0) {
            // Receive role from the server
            memset(role, 0, sizeof(role));
            if (recv(sockfd, role, sizeof(role), 0) <= 0) {
                perror("Error receiving role");
                close(sockfd);
                exit(0);
            }
            printf("Logged in as: %s\n", role);

            // Start menu interaction
            while (1) {
                char menu[MAX_BUFFER];
                memset(menu, 0, sizeof(menu));

                // Receive menu from the server
                if (recv(sockfd, menu, sizeof(menu), 0) <= 0) {
                    perror("Error receiving menu");
                    close(sockfd);
                    exit(0);
                }
                printf("%s\n", menu);

                // Input user choice
                printf("Enter your choice (or enter 0 to logout): ");
                scanf("%d", &choice);

                // Send choice to the server
                if (send(sockfd, &choice, sizeof(choice), 0) <= 0) {
                    perror("Error sending choice");
                    close(sockfd);
                    exit(0);
                }

                // Handle exit condition (logging out)
                if (choice == 0) {
                    printf("Logging out...\n");
                    break;  // Exit the inner menu loop
                }

                // Process role-specific actions based on the choice
                if (strcmp(role, "Customer") == 0) {
                    customerCase(username, choice);
                } 
                else if(strcmp(role,"Employee") == 0){
                    empCase(username,choice);
                }
                else if(strcmp(role,"Manager") == 0){
                    manCase(username,choice);
                }
                else if(strcmp(role,"Admin") == 0){
                    adminCase(username,choice);
                }


            }

<<<<<<< HEAD
        } else {
            printf("Login failed. Please try again.\n");
=======
            else if (strcmp(buffer,"Manager")==0){
                memset(buffer, 0, sizeof(buffer)); 
                displayManMenu(username);
            }
            else if (strcmp(buffer,"Admin") == 0){
                memset(buffer, 0, sizeof(buffer)); 
                displayAdminMenu(username);
            }

            

            break;
>>>>>>> eb84e4dd2cdbdde4094cb23157fe8cbad42d2cda
        }

        printf("Do you want to try logging in again? (y/n): ");
        char retry;
        scanf(" %c", &retry); 
        if (retry == 'n' || retry == 'N') {
            printf("Exiting the client.\n");
            break;
        }
    }while(1);

    close(sockfd);
>>>>>>> 2e475b2db8a3364cdb5353b4c7ab08ce150de7cd
    return 0;
}
