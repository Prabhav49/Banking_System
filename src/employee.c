#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.h"
#include <unistd.h> 
#include "banking.h"
#include <fcntl.h>    
#include <sys/file.h> 
#include <time.h>
#include "transaction.h"
#include "employee.h"
#include "loan.h"

#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db" 


void displayEmployeeMenu(const char *username) {
    int choice;

    while (1) {
        printf("\nEmployee Menu:\n");
        printf("1. Add New Customer\n");
        printf("2. Modify Customer Detail\n");
        printf("3. Process Loan Application\n");
        printf("4. Approve/Reject Loans\n");
        printf("5. View Assigned Loan Applications\n");
        printf("6. View Customer Transaction\n");
        printf("7. Change Password\n");
        printf("8. Logout\n");
        printf("9. Exit\n");
        printf("10. User Info\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);



        switch (choice) {
            case 1:
                addNewCustomer();
                break;
            case 2:
                modifyCustomerDetail();
                break;
            case 3:
                processLoanApplication(username);
                break;
            case 4:
                approveOrRejectLoanApp(username);
                break;
            case 5:
                viewAssignedLoans(username);
                break;
            case 6:
                viewCustTrans();
                break;
            case 7:
                changeEmpPassword(username);
                break;
            case 8:
                printf("Logging Out");
                exit(0);
            case 9:
                exit(0); 
            case 10:
                empInfo(username); 
                break;    
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

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
    return maxId + 1; 
}
int isUsernameUnique(const char *username) {
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        return 1; // If the file doesn't exist, assume username is unique
    }

    User user;
    // Check if the username already exists in the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            fclose(file);
            return 0; // Username already exists
        }
    }

    fclose(file);
    return 1; // Username is unique
}
void addCust(const char *fullName, const char *username, const char *password, const char *role, double balance, int active) {
    User user;
    user.id = generateUserId(); // Generate a unique ID
    strncpy(user.fullName, fullName, sizeof(user.fullName) - 1);
    strncpy(user.username, username, sizeof(user.username) - 1);
    strncpy(user.password, password, sizeof(user.password) - 1);
    strncpy(user.role, role, sizeof(user.role) - 1);
    user.balance = balance;
    user.active = active;

    // Open the database file in append mode
    FILE *file = fopen("../db/users.db", "ab"); // Open for appending in binary mode
    if (!file) {
        perror("Failed to open users.db");
        exit(EXIT_FAILURE);
    }
    fwrite(&user, sizeof(User), 1, file);
    fclose(file);
}
void addNewCustomer(){
    char fullName[100];
    char username[50];
    char password[50];
    char role[20];
    double balance;
    int active;

  
    // Consume the leftover newline character from the previous input
    getchar(); 
    // Prompt for a unique username
    printf("\nEnter Full Name: ");
    scanf("%99[^\n]", fullName);
    getchar(); 
    do {
        printf("Enter Username: ");
        scanf("%49s", username);

        if (!isUsernameUnique(username)) {
            printf("Error: Username '%s' already exists. Please enter a unique username.\n", username);
        }
    } while (!isUsernameUnique(username)); 


    printf("Enter Password: ");
    scanf("%49s", password);

    strcpy(role, "Customer");
    // printf("Enter Role (Customer/Employee): ");
    // scanf("%19s", role);

    printf("Enter Balance: ");
    scanf("%lf", &balance);

    printf("Is the account active? (1 for Yes, 0 for No): ");
    scanf("%d", &active);

    // Add the user to the database
    addCust(fullName, username, password, role, balance, active);

    printf("User added successfully!\n");

    return;
}

void modifyCustomerDetail() {
    char username[50];
    User user;
    int found = 0;

    // Prompt for the username to modify details
    printf("Enter the username of the customer to modify: ");
    scanf("%49s", username);

    // Open the database file in read/write mode
    FILE *file = fopen("../db/users.db", "r+b");
    if (!file) {
        perror("Failed to open users.db");
        return;
    }

    // Search for the user in the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            break;
        }
    }

    // If user is found, prompt for details to modify
    if (found) {
        printf("User found. You can modify the following details:\n");

        // Modify Full Name
        printf("Enter new Full Name (currently: %s): ", user.fullName);
        getchar();  // To clear newline from previous input
        fgets(user.fullName, sizeof(user.fullName), stdin);
        user.fullName[strcspn(user.fullName, "\n")] = 0; // Remove trailing newline

        // Modify Password
        printf("Enter new Password (currently: %s): ", user.password);
        scanf("%49s", user.password);

        // Modify Balance
        printf("Enter new Balance (currently: %.2f): ", user.balance);
        scanf("%lf", &user.balance);

        // Modify Account Status
        printf("Is the account active? (currently: %d, 1 for Yes, 0 for No): ", user.active);
        scanf("%d", &user.active);

        // Move file pointer to the start of the current user record
        fseek(file, -sizeof(User), SEEK_CUR);

        // Update the user details in the file
        fwrite(&user, sizeof(User), 1, file);

        printf("Customer details updated successfully!\n");
    } else {
        printf("Error: User with username '%s' not found.\n", username);
    }

    // Close the file
    fclose(file);
}


void processLoanApplication(const char *username){
    printf("Yet to implement");
}

void approveOrRejectLoanApp(const char *username){
    printf("Yet to implement");
}
void viewAssignedLoans(const char *username){
    printf("Yet to implement");
}

void viewCustTrans() {
    char username[50];
    User user;
    int found = 0;

    // Prompt for the username to view transaction history
    printf("Enter the username of the customer to view transaction history: ");
    scanf("%49s", username);

    // Open the user database file in read mode
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        perror("Failed to open user database");
        return;
    }

    // Find user by username
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            break;   
        }
    }

    fclose(file);

    if (!found) {
        printf("User not found.\n");
        return;
    }

    // Now fetch transaction history for found user
    file = fopen("../db/transactions.db", "rb");
    if (!file) {
        perror("Failed to open transactions database");
        return;
    }

    Transaction transaction;
    int foundTransactions = 0;

    printf("Transaction History for User %s (ID: %d):\n", username, user.id);
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-15s %-15s %-15s %-10s %s\n", "Transaction ID", "Sender ", "Receiver ", "Type", "Amount", "Date/Time");
    printf("--------------------------------------------------------------------------------------------------\n");

    // Read all transactions and filter by user ID
    while (fread(&transaction, sizeof(Transaction), 1, file)) {
        if (strcmp(transaction.tranUser ,user.username ) == 0) {
            foundTransactions = 1; 
            printf("%-15d %-15s %-15s %-15s %.2f %s\n", 
                   transaction.transactionId, 
                   transaction.sendUser, 
                   transaction.recUser, 
                   transaction.type, 
                   transaction.amount, 
                   transaction.dateTime);
        }
    }

    if (!foundTransactions) {
        printf("No transactions found for User %s.\n", username);
    }

    fclose(file);
}


void changeEmpPassword(const char *username){
    char currentPassword[MAX_PASSWORD_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];
    char confirmPassword[MAX_PASSWORD_LENGTH];
    FILE *file = fopen(USER_DB_PATH, "r+b");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User user;
    int found = 0;

    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1; // User found
            break;
        }
    }
    if (!found) {
        printf("User not found.\n");
        fclose(file);
        return;
    }

    printf("Enter current password: ");
    scanf("%s", currentPassword);

    // Validate the current password
    if (strcmp(user.password, currentPassword) != 0) {
        printf("Incorrect current password.\n");
        fclose(file);
        return;
    }

    printf("Enter new password: ");
    scanf("%s", newPassword);
    printf("Confirm new password: ");
    scanf("%s", confirmPassword);

    // Check if the new password and confirmation match
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("New passwords do not match.\n");
        fclose(file);
        return;
    }

    // Check if new password is valid 
    if (strlen(newPassword) < 6) { 
        printf("New password must be at least 6 characters long.\n");
        fclose(file);
        return;
    }

    // Update the password in the user record
    strcpy(user.password, newPassword);
    fseek(file, -sizeof(User), SEEK_CUR); 
    size_t written = fwrite(&user, sizeof(User), 1, file); 
    // Check if the write was successful
    if (written != 1) {
        printf("Failed to update password in the database. Write returned: %zu\n", written);
    } else {
        printf("Password changed successfully.\n");
    }

    fclose(file);
}


void empInfo(const char *username){
    FILE *file = fopen("../db/users.db", "r");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User user;
    int found = 0;

    // Read each user from the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            found = 1; 
            break;   
        }
    }

    fclose(file);

    // Print user information if found
    if (found) {
        printf("User Information:\n");
        printf("ID: %d\n", user.id);
        printf("Name: %s\n", user.fullName);
        printf("Username: %s\n", user.username);
        printf("Role: %s\n", user.role);
        printf("Balance: %.2f\n", user.balance);
        printf("Account Status: %s\n", user.active ? "Active" : "Inactive");
    } else {
        printf("User not found.\n");
    }
}