#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banking.h"

#define USER_DB_PATH "../db/users.db"

// Function to display all users in a table format
void displayUsersInTable() {
    User user;
    FILE *file = fopen(USER_DB_PATH, "rb");
    
    if (file == NULL) {
        printf("Error: Could not open the users database.\n");
        return;
    }

    // Print table header
    printf("+----+---------------------------+------------------+------------------+------------------+-----------+-------------+---------+--------+\n");
    printf("| ID | Full Name                 | Username         | Password         | Role             | Balance   | Status      | Active  |  Status \n");
    printf("+----+---------------------------+------------------+------------------+------------------+-----------+-------------+---------+--------+\n");

    // Read and display all records
    while (fread(&user, sizeof(User), 1, file)) {
        printf("| %-2d | %-25s | %-16s | %-16s | %-16s | %-9.2f | %-11s | %-7s | %-7s |\n",
            user.id,
            user.fullName,
            user.username,
            user.password,    // Displaying password here
            user.role,
            user.balance,
            user.active ? "Active" : "Inactive",
            user.active ? "Yes" : "No",
            user.isLogIn? "True": "False");
    }

    // Print table footer
    printf("+----+---------------------------+------------------+------------------+------------------+-----------+-------------+---------+\n");

    fclose(file);
}

int main() {
    displayUsersInTable();
    return 0;
}