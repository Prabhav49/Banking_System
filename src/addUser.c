#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banking.h"
#include <stdbool.h>

// Function to generate a unique user ID
int generateUserId() {
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        return 1; // Start from 1 if the file doesn't exist
    }
    
    User user;
    int maxId = 0;

    // Read existing users to find the maximum ID
    while (fread(&user, sizeof(User), 1, file)) {
        if (user.id > maxId) {
            maxId = user.id;
        }
    }

    fclose(file);
    return maxId + 1; // Return the next available ID
}

// Function to add a user to the database
void addUser(const char *fullName, const char *username, const char *password, const char *role, double balance, int active, bool  isLogIn) {
    User user;
    user.id = generateUserId(); // Generate a unique ID
    strncpy(user.fullName, fullName, sizeof(user.fullName) - 1);
    strncpy(user.username, username, sizeof(user.username) - 1);
    strncpy(user.password, password, sizeof(user.password) - 1);
    strncpy(user.role, role, sizeof(user.role) - 1);
    user.balance = balance;
    user.active = active;
    user.isLogIn = false;

    // Open the database file in append mode
    FILE *file = fopen("../db/users.db", "ab"); // Open for appending in binary mode
    if (!file) {
        perror("Failed to open users.db");
        exit(EXIT_FAILURE);
    }

    // Write the new user to the database
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
}

int main() {
    char fullName[100];
    char username[50];
    char password[50];
    char role[20];
    double balance;
    int active;
    bool isLogIn;

    // Prompt for user information
    printf("Enter Full Name: ");
    scanf("%99[^\n]", fullName);
    getchar(); // Clear the newline character from input buffer

    printf("Enter Username: ");
    scanf("%49s", username);

    printf("Enter Password: ");
    scanf("%49s", password);

    printf("Enter Role (Customer/Employee): ");
    scanf("%19s", role);

    printf("Enter Balance: ");
    scanf("%lf", &balance);

    printf("Is the account active? (1 for Yes, 0 for No): ");
    scanf("%d", &active);
    isLogIn = false;


    // Add the user to the database
    addUser(fullName, username, password, role, balance, active,isLogIn);

    printf("User added successfully!\n");

    return 0;
}