#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "banking.h"
#include "customer.h"

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char username[50], password[50], buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with the server failed");
        exit(0);
    } else {
        printf("Connected to the server\n");
    }

    while (1) {
        // Ask for username and password
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        // Send username and password to server
        send(sockfd, username, sizeof(username), 0);
        send(sockfd, password, sizeof(password), 0);

        // Clear the buffer before receiving a response
        memset(buffer, 0, sizeof(buffer));

        // Receive server response
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);

        // If login is successful, break the loop
        if (strcmp(buffer, "Login Failed!") != 0) {
            memset(buffer, 0, sizeof(buffer));
            recv(sockfd, buffer, sizeof(buffer), 0);
            if (strcmp(buffer, "Customer") == 0) {
                displayCustomerMenu(username);
            }
            break;
        }

        printf("Login failed. Please try again.\n");
    }

    close(sockfd); // Close connection after successful login
    return 0;
}
