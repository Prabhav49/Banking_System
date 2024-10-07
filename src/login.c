#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "banking.h"

// Function to authenticate from the database
const char* authenticate(const char *username, const char *password) {
    FILE *file = fopen("../db/users.db", "r");

    if (!file) {
        perror("Failed to open database");
        return "Login Failed!";
    }

    static char line[256];  // To store each line from the database
    static char msg[50];    // To store the response message

    // Read through the database file to find matching username and password
    while (fgets(line, sizeof(line), file)) {
        char *id = strtok(line, "|");
        char *name = strtok(NULL, "|");
        char *db_username = strtok(NULL, "|");
        char *db_password = strtok(NULL, "|");
        char *role = strtok(NULL, "|");
        char *balance = strtok(NULL, "|");
        char *active = strtok(NULL, "|");  // Account active status

        // Remove any trailing newline or space characters from the active status
        active[strcspn(active, "\n")] = '\0';  // Trim newline

        // Debugging output
        printf("Checking user: %s, active: %s\n", db_username, active);  // Debug line

        // Check if username and password match
        if (strcmp(db_username, username) == 0 && strcmp(db_password, password) == 0) {
            // Check if the account is active
            if (strcmp(active, "1") == 0) {
                sprintf(msg, "%s Login Successful!", role);
            } else {
                strcpy(msg, "Login Failed! Account is inactive.");
            }
            fclose(file);
            return msg;
        }
    }

    fclose(file);
    return "Login Failed!";
}
