#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/file.h>
#include "banking.h"

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

    // Load users from the database
    User users[MAX_USERS];
    int userCount = 0;
    loadUsers(users, &userCount);

    while (1) {
        connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (connfd < 0) {
            perror("Server accept failed");
            continue;
        }
        printf("Server accepted the client\n");

        char username[50], password[50];

        // Receive username and password
        recv(connfd, username, sizeof(username), 0);
        recv(connfd, password, sizeof(password), 0);

        // Authenticate the user
        const char *msg = authenticate(username, password, users, userCount);
        send(connfd, msg, strlen(msg), 0);

        if (strcmp(msg, "Login Successful!") == 0) {
            printf("%s logged in.\n", username);

            // Get user role and send to client
            const char *role = checkRole(username, users, userCount);
            send(connfd, role, strlen(role) + 1, 0);
        }

        // Close connection for the current client
        close(connfd);
        printf("Client connection closed\n");

    }

    close(sockfd);  // Close server socket
    return 0;
}
