// /src/server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "banking.h"

// No need to include login.c here

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    } else {
        printf("Socket successfully created\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        exit(0);
    } else {
        printf("Socket successfully bound\n");
    }

    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(0);
    } else {
        printf("Server listening\n");
    }

    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        perror("Server accept failed");
        exit(0);
    } else {
        printf("Server accepted the client\n");
    }

    char username[50], password[50];
    recv(connfd, username, sizeof(username), 0);
    recv(connfd, password, sizeof(password), 0);

    const char *msg = authenticate(username, password);
    send(connfd, msg, strlen(msg), 0);

    close(sockfd);
    return 0;
}
