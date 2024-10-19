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
#include "employee.h"
#include <stdbool.h>


#define PORT 8080
int choice;

void sendLogoutStatus(int client_socket, bool loggedOut) {
    int statusToSend = loggedOut ? 1 : 0;
    
    // Send the integer value to the server
    if (write(client_socket, &statusToSend, sizeof(statusToSend)) < 0) {
        perror("Failed to send logout status to server");
    }
}

bool handleChoice(const char*username, int choice, const char* role){

    if(strcmp(role,"Customer")==0){
        customerCase(username,choice);
        if(choice == 11 ) return true;
        else return false;
    }
    else if(strcmp(role,"Employee")==0){
        empCase(username,choice);
        if(choice == 8 ) return true;
        else return false;
    }
    else if(strcmp(role,"Admin")==0){
        adminCase(username,choice);
        if(choice == 5 ) return true;
        else return false;
    }
    else if(strcmp(role,"Manager")==0){
        manCase(username,choice);
        if(choice == 5 ) return true;
        else return false;
    }
    
}

void displayMenuBasedOnRole(const char* buffer) {

    if (strcmp(buffer, "Customer") == 0) {
        char* menu = getCustomerMenu();
        printf("%s", menu); 
        free(menu);    
    }
    else if (strcmp(buffer, "Manager") == 0) {
        char* menu = getManagerMenu();
        printf("%s", menu);  
        free(menu);
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
        username[strcspn(username, "\n")] = 0; 
        write(client_socket, username, strlen(username));

        // Display and input password from client side
        printf("Enter Password: ");
        if (fgets(password, sizeof(password), stdin) == NULL) {
            printf("Error reading password.\n");
            continue;
        }
        password[strcspn(password, "\n")] = 0; 
        write(client_socket, password, strlen(password));

        // Receive and display the authentication message from the server
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            printf("Server disconnected.\n");
            break; 
        }
        buffer[bytes_read] = '\0'; 
        //Auth result 
        printf("%s\n", buffer); 

        // Check if authentication was successful
        if (strcmp(buffer, "Login Successful!\n") == 0) {
            // Receive the role from the server
            memset(role, 0, sizeof(role));
            bytes_read = read(client_socket, role, sizeof(role) - 1);
            if (bytes_read <= 0) {
                printf("Server disconnected while sending role.\n");
                break;
            }
            role[bytes_read] = '\0';  

            // Print the role
            printf("Role: %s\n", role);
            while (1) {
                displayMenuBasedOnRole(role);
                int choice;
                printf("Enter Your Choice : ");
                scanf("%d", &choice);
                while (getchar() != '\n');
                if(handleChoice(username, choice, role)){
                    break;
                }
                else continue;
            }
            sendLogoutStatus(client_socket, true);

        } else {
            continue;
        }
    }
}


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

    close(client_socket);
    return 0;
}
