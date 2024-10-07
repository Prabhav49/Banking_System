// /src/client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "banking.h"

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

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with the server failed");
        exit(0);
    } else {
        printf("Connected to the server\n");
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    send(sockfd, username, sizeof(username), 0);
    send(sockfd, password, sizeof(password), 0);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    close(sockfd);
    return 0;
}
