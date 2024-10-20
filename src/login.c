#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "banking.h"

#define MAX_USERS 100

// Function to load users from the users.db file
void loadUsers(User users[], int *userCount) {
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        perror("Failed to open users.db");
        exit(EXIT_FAILURE);
    }

    *userCount = fread(users, sizeof(User), MAX_USERS, file);
    fclose(file);
}

// Function to save updated users to the users.db file
void saveUsers(User users[], int userCount) {
    FILE *file = fopen("../db/users.db", "wb");
    if (!file) {
        perror("Failed to open users.db for writing");
        exit(EXIT_FAILURE);
    }

    fwrite(users, sizeof(User), userCount, file);
    fclose(file);
}

// Function to authenticate a user, updating the isLogIn status if successful
const char* authenticate(const char *username, const char *password, User users[], int userCount) {
    static char msg[100];  // Static to persist after function return
    int usernameFound = 0;

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            usernameFound = 1;  // Username exists
            if (strcmp(users[i].password, password) == 0) {
                // Password matches, now check account status
                if (users[i].active == 1) {
                    if (users[i].isLogIn == false) {
                        users[i].isLogIn = true;  // Mark user as logged in
                        saveUsers(users, userCount);  // Save updated status to file
                        strcpy(msg, "Login Successful!");
                    } else {
                        strcpy(msg, "Login Failed! User is already logged in.");
                    }
                } else {
                    strcpy(msg, "Login Failed! Account is inactive.");
                }
            } else {
                strcpy(msg, "Password is incorrect.");
            }
            return msg;
        }
    }

    if (!usernameFound) {
        strcpy(msg, "Username does not exist.");
    }

    return msg;
}


void logout(const char *username) {
    User users[MAX_USERS];
    int userCount = 0;
    int usernameFound = 0;

    // Load users from the file
    loadUsers(users, &userCount);

    // Search for the user in the loaded data
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            usernameFound = 1;  // Username exists
            if (users[i].isLogIn == true) {
                users[i].isLogIn = false;  // Mark user as logged out
                saveUsers(users, userCount);  // Save updated status to file
                printf("%s Logged Out....\n", username);
            } else {
                printf("%s is not logged in..\n", username);
            }
            return;  // Exit the function after processing the user
        }
    }

    if (!usernameFound) {
        printf("User does not exist\n");
    }
}

// Function to check the role of a user
const char* checkRole(const char *username, User users[], int userCount) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return users[i].role;
        }
    }
    return "Login Failed!";
}
