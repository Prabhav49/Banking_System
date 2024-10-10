#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "banking.h"

const char* authenticate(const char *username, const char *password, User users[], int userCount) {
    static char msg[50];

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            if (users[i].active == 1) {
                strcpy(msg, "Login Successful!");
            } else {
                strcpy(msg, "Login Failed! Account is inactive.");
            }
            return msg;
        }
    }
    return "Login Failed!";
}

void loadUsers(User users[], int *userCount) {
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        perror("Failed to open users.db");
        exit(EXIT_FAILURE);
    }

    *userCount = fread(users, sizeof(User), MAX_USERS, file);
    fclose(file);
}

const char* checkRole(const char *username, User users[], int userCount) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return users[i].role;
        }
    }
    return "Login Failed!";
}