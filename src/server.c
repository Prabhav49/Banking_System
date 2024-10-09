#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/file.h>
#include "banking.h"

int lockUserRecord(int fd, int recordIndex) {
    struct flock lock;
    lock.l_type = F_WRLCK;   // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = recordIndex * sizeof(User);  // Lock the specific record
    lock.l_len = sizeof(User);  // Length of the record
    lock.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &lock);
}

int unlockUserRecord(int fd, int recordIndex) {
    struct flock lock;
    lock.l_type = F_UNLCK;   // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = recordIndex * sizeof(User);  // Unlock the specific record
    lock.l_len = sizeof(User);  // Length of the record
    lock.l_pid = getpid();

    return fcntl(fd, F_SETLK, &lock);
}

int findUserIndex(const char *username, User users[], int userCount) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

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

        // Find user index for locking
        int userIndex = findUserIndex(username, users, userCount);
        if (userIndex == -1) {
            const char *msg = "Login Failed! User not found.";
            send(connfd, msg, strlen(msg), 0);
            close(connfd);
            continue;
        }

        // Lock the user record
        int fd = open("../db/users.db", O_RDWR);
        if (lockUserRecord(fd, userIndex) == -1) {
            perror("Failed to lock user record");
            close(fd);
            close(connfd);
            continue;
        }

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

        // Unlock the user record after client logs out
        if (unlockUserRecord(fd, userIndex) == -1) {
            perror("Failed to unlock user record");
        }

        close(fd);
    }

    close(sockfd);  // Close server socket
    return 0;
}
