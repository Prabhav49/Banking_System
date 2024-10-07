#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "banking.h"

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    int opt = 1;

    // Create socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }

    // Set socket options to reuse the address and port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the specified port and address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        exit(0);
    } else {
        printf("Socket successfully bound\n");
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(0);
    } else {
        printf("Server listening\n");
    }

    socklen_t len = sizeof(cli);

    // Server loop to accept new clients
    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            perror("Server accept failed");
            continue;
        } else {
            printf("Server accepted the client\n");
        }

        // For each client, handle authentication attempts
        char username[50], password[50];

        // Receive username and password from the client
        recv(connfd, username, sizeof(username), 0);
        recv(connfd, password, sizeof(password), 0);

        // Authenticate the user using the database
        const char *msg = authenticate(username, password);
        send(connfd, msg, strlen(msg), 0);

        // If login is successful, proceed with role-based functionality
        if (strstr(msg, "Login Successful!") != NULL) {
            printf("%s logged in.\n", username);
            // Proceed to role-based functionality (to be added in future modules)
        }

        close(connfd); // Close connection for the current client and wait for the next client
        printf("Client connection closed\n");
    }

    close(sockfd); // Close server socket when done
    return 0;
}
