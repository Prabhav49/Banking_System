#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "banking.h"
#include "customer.h"
#include "employee.h"
#include "manager.h"
#include "admin.h"

#define MAX_BUFFER 1024

int main() {
    int sockfd, choice;
    struct sockaddr_in servaddr;
    char username[50], password[50], buffer[MAX_BUFFER], role[50];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }

    // Initialize server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT);  

    // Attempt to connect to the server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection to the server failed");
        close(sockfd);
        exit(0);
    } else {
        printf("Connected to the server\n");
    }

    do{
        // Clear previous input data
        memset(username, 0, sizeof(username));
        memset(password, 0, sizeof(password));
        memset(buffer, 0, sizeof(buffer));

        // Receive username prompt from server
        if (recv(sockfd, buffer, sizeof(buffer), 0) <= 0) {
            perror("Error receiving username prompt");
            close(sockfd);
            exit(0);
        }
        printf("%s", buffer);

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

        } else {
            printf("Login failed. Please try again.\n");
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
    return 0;
}
