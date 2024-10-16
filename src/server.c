#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "banking.h"
#include "menu.h"
#include "customer.h"

void handleClient(int connfd);

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    int opt = 1;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    // Set socket options
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully bound\n");

    // Listen for connections
    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening\n");

    socklen_t len = sizeof(cli);

    // Server loop to accept multiple clients
    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            perror("Server accept failed");
            continue;
        }
        printf("Server accepted the client\n");

        // Fork a new process to handle the client
        pid_t pid = fork();
        if (pid == 0) {
            close(sockfd); 
            handleClient(connfd); 
            close(connfd); 
            exit(0);
        } else if (pid > 0) { 
            close(connfd); 
            waitpid(-1, NULL, WNOHANG);
        } else {
            perror("Fork failed");
            close(connfd);
        }
    }

    close(sockfd);
    return 0;
}

void handleClient(int connfd) {
    User users[MAX_USERS];
    int userCount = 0;
    loadUsers(users, &userCount);

    char username[50], password[50];
    int loginAttempts = 0;
    char retryChoice[2]; // For storing 'y' or 'n'

    while (loginAttempts < 3) {
        const char *usernamePrompt = "Enter username: ";
        if (send(connfd, usernamePrompt, strlen(usernamePrompt) + 1, 0) <= 0) {
            perror("Error sending username prompt");
            close(connfd);
            return;
        }

        // Receive username
        memset(username, 0, sizeof(username));
        recv(connfd, username, sizeof(username), 0);

        // Send password prompt
        const char *passwordPrompt = "Enter password: ";
        send(connfd, passwordPrompt, strlen(passwordPrompt) + 1, 0);

        // Receive password
        memset(password, 0, sizeof(password));
        recv(connfd, password, sizeof(password), 0);

        // Authenticate the user
        const char *msg = authenticate(username, password, users, userCount);
        send(connfd, msg, strlen(msg) + 1, 0);

        // Check if login was successful
        if (strcmp(msg, "Login Successful!") == 0) {
            printf("%s logged in.\n", username);

            // Send the role to the client
            const char *role = checkRole(username, users, userCount);
            if (send(connfd, role, strlen(role) + 1, 0) <= 0) {
                perror("Error sending role");
                close(connfd);
                return;
            }

            // Send menu and process choice
            char *menu = NULL;
            do {
                // Select the correct menu based on the role
                if (strcmp(role, "Customer") == 0) {
                    menu = getCustomerMenu();
                } else if (strcmp(role, "Employee") == 0) {
                    menu = getEmployeeMenu();
                } else if (strcmp(role, "Manager") == 0) {
                    menu = getManagerMenu();
                } else if (strcmp(role, "Admin") == 0) {
                    menu = getAdminMenu();
                } else {
                    menu = strdup("Invalid role");
                }

                // Send menu to client
                if (send(connfd, menu, strlen(menu) + 1, 0) <= 0) {
                    perror("Error sending menu");
                    free(menu);
                    close(connfd);
                    return;
                }

                // Receive the user's choice
                int choice;
                if (recv(connfd, &choice, sizeof(choice), 0) <= 0) {
                    perror("Error receiving choice");
                    free(menu);
                    close(connfd);
                    return;
                }

                // Handle choice processing
                printf("User %s made a choice: %d\n", username, choice);

                free(menu);
            } while (1);

            return;
        } else {
            loginAttempts++;

            // Check if the user has another attempt left
            if (loginAttempts < 3) {
                const char *retryMsg = "Login failed. Do you want to try logging in again? (y/n): ";
                send(connfd, retryMsg, strlen(retryMsg) + 1, 0);

                // Receive retry choice
                memset(retryChoice, 0, sizeof(retryChoice));
                recv(connfd, retryChoice, sizeof(retryChoice), 0);

                // If the user chooses 'n', disconnect the client
                if (strcmp(retryChoice, "n") == 0) {
                    const char *exitMsg = "Goodbye!";
                    send(connfd, exitMsg, strlen(exitMsg) + 1, 0);
                    close(connfd);
                    return;
                } else if (strcmp(retryChoice, "y") != 0) {
                    // If invalid input (anything other than 'y'), disconnect
                    const char *invalidChoiceMsg = "Invalid choice. Disconnecting.";
                    send(connfd, invalidChoiceMsg, strlen(invalidChoiceMsg) + 1, 0);
                    close(connfd);
                    return;
                }
            } else {
                const char *failMsg = "Login failed 3 times. Disconnecting.";
                send(connfd, failMsg, strlen(failMsg) + 1, 0);
                close(connfd);
                return;
            }
        }
    }

    close(connfd); // Close the connection after 3 failed attempts
}