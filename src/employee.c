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
#include "customer.h"

#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db" 


// void displayEmployeeMenu(const char *username) {
//     int choice;

//     while (1) {
//         printf("\nEmployee Menu:\n");
//         printf("1. Add New Customer\n");
//         printf("2. Modify Customer Detail\n");
//         printf("3. Process Loan Application\n");
//         printf("4. Approve/Reject Loans\n");
//         printf("5. View Assigned Loan Applications\n");
//         printf("6. View Customer Transaction\n");
//         printf("7. Change Password\n");
//         printf("8. Logout\n");
//         printf("9. Exit\n");
//         printf("10. User Info\n");
//         printf("Enter your choice: ");
//         scanf("%d", &choice);



//         switch (choice) {
//             case 1:
//                 addNewCustomer();
//                 break;
//             case 2:
//                 modifyCustomerDetail();
//                 break;
//             case 3:
//                 processLoanApplication(username);
//                 break;
//             case 4:
//                 approveOrRejectLoanApp(username);
//                 break;
//             case 5:
//                 viewAssignedLoans(username);
//                 break;
//             case 6:
//                 viewCustTrans();
//                 break;
//             case 7:
//                 changePassword(username);
//                 break;
//             case 8:
//                 printf("Logging Out");
//                 exit(0);
//             case 9:
//                 exit(0); 
//             case 10:
//                 userInfo(username); 
//                 break;    
//             default:
//                 printf("Invalid choice! Please try again.\n");
//         }
//     }
// }


void empCase(const char*username, int choice){
    printf(" \n");
    printf("===========YOUR QUERY RESULT===========\n");
    printf("--------------------------------------------------------\n");
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
                changePassword(username);
                break;
            case 8:
                logout(username);
                break;
            case 9:
                userInfo(username); 
                break;    
            default:
                printf("Invalid choice! Please try again.\n");
    }
    printf("--------------------------------------------------------\n");
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
    user.isLogIn = false;
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

    printf("Press Enter to start\n");
    // Consume the leftover newline character from the previous input
    getchar(); 

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

    balance = 0.00;

    printf("Is the account active? (1 for Yes, 0 for No): ");
    scanf("%d", &active);

    // Add the user to the database
    addCust(fullName, username, password, role, balance, active);

    printf("User added successfully!\n");

    return;
}

void modifyCustomerDetail() {
     User userRecord;
    char username[50];
    int found = 0;

    // Ask for the username whose details need to be modified
    printf("Enter the username of the Customer to modify: ");
    scanf("%s", username);

    FILE *userFile = fopen(USER_DB_PATH, "rb+"); 
    if (userFile == NULL) {
        printf("Error: Could not open user database.\n");
        return;
    }

    // Loop through the user database to find the matching username
    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0) {
            if (strcmp(userRecord.role, "Employee") == 0 || strcmp(userRecord.role, "Manager") == 0 || strcmp(userRecord.role, "Admin") == 0)  {
                printf("Error: You cannot modify the details of an Employee/Manager/Admin user.\n");
                fclose(userFile);
                return;
            }

            found = 1;

            // Display current information of the customer/employee
            printf("\nCurrent Information for Username: %s\n", userRecord.username);
            printf("Full Name: %s\n", userRecord.fullName);
            printf("Role: %s\n", userRecord.role);
            printf("Balance: %.2f\n", userRecord.balance);
            printf("Account Status: %s\n", userRecord.active ? "Active" : "Inactive");
            printf("-------------------------------------\n");

            // Ask which details to modify (without balance modification)
            char choice;
            printf("Do you want to modify the full name? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new full name: ");
                getchar();  // Consume leftover newline character
                fgets(userRecord.fullName, sizeof(userRecord.fullName), stdin);
                userRecord.fullName[strcspn(userRecord.fullName, "\n")] = '\0';  // Remove newline
            }

            printf("Do you want to modify the password? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("Enter new password: ");
                scanf("%s", userRecord.password);
            }

            printf("Do you want to change the account status? (1 for active, 0 for inactive): ");
            scanf("%d", &userRecord.active);

            // Move the file pointer back by one record to overwrite the current record
            fseek(userFile, -sizeof(User), SEEK_CUR);
            fwrite(&userRecord, sizeof(User), 1, userFile);
            fflush(userFile);

            printf("Details updated successfully for username: %s\n", username);
            break;
        }
    }

    if (!found) {
        printf("Error: Username '%s' not found.\n", username);
    }

    fclose(userFile);
}


void processLoanApplication(const char *username) {
    User userRecord;
    Loan loanRecord;
    char employeeId[30]; // To store the assigned employee's ID
    int found = 0; // To check if the employee was found
    int loanToProcess;
    double newInterestRate;
    int newDuration;

    // Step 1: Find the employee ID based on the username from the user database
    FILE *userFile = fopen(USER_DB_PATH, "rb");
    if (userFile == NULL) {
        printf("Error: Could not open users database.\n");
        return;
    }

    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0 && strcmp(userRecord.role, "Employee") == 0) {
            // Convert employee ID to string format
            sprintf(employeeId, "%d", userRecord.id); // Store the ID as a string
            found = 1;
            break;
        }
    }
    fclose(userFile);

    if (!found) {
        printf("Error: Employee with username '%s' not found or is not an employee.\n", username);
        return;
    }

    // Step 2: Process loans assigned to this employee
    FILE *loanFile = fopen(LOAN_DB_PATH, "rb+");
    if (loanFile == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }

    // Display header for the table
        printf("Loans Assigned to Employee (ID: %s) for Processing:\n", employeeId);
        printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
        printf("| Loan ID |   Loan Amount    |   Status   |  Duration  |  Interest Rate  |   Username    |        User ID           |\n");
        printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");

        // Loop through loans and find ones with "Processing" status and assigned to this employee
        found = 0;  // Reset found flag
        while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
            if (strcmp(loanRecord.assignedEmployeeId, employeeId) == 0 && strcmp(loanRecord.status, "Processing") == 0) {
                printf("| %-7d | %15.2lf | %-10s | %-10d | %15.2f%% | %-13s | %-23s |\n",
                    loanRecord.loanId,
                    loanRecord.loanAmount,
                    loanRecord.status,
                    loanRecord.durationMonths,
                    loanRecord.interestRate,
                    loanRecord.username[0] == '\0' ? "N/A" : loanRecord.username,
                    loanRecord.userId[0] == '\0' ? "N/A" : loanRecord.userId);
                found = 1;
            }
        }
        printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");


    if (!found) {
        printf("No loans in 'Processing' status assigned to you.\n");
        fclose(loanFile);
        return;
    }

    // Ask for the loan ID to process
    printf("Enter the Loan ID you want to process: ");
    scanf("%d", &loanToProcess);

    // Rewind the file pointer to start from the beginning
    rewind(loanFile);

    // Search for the loan by ID and assigned employee ID
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (loanRecord.loanId == loanToProcess && strcmp(loanRecord.assignedEmployeeId, employeeId) == 0) {
            // Ask for new interest rate and duration
            printf("Enter new interest rate: ");
            scanf("%lf", &newInterestRate);
            printf("Enter new duration (in months): ");
            scanf("%d", &newDuration);

            // Update loan details
            loanRecord.interestRate = newInterestRate;
            loanRecord.durationMonths = newDuration;

            // Move the file pointer back by one record to overwrite the current loan record
            fseek(loanFile, -sizeof(Loan), SEEK_CUR);
            fwrite(&loanRecord, sizeof(Loan), 1, loanFile);
            fflush(loanFile);

            printf("Loan ID %d has been updated. Status remains: Processing.\n", loanToProcess);
            fclose(loanFile);
            return;
        }
    }

    // If loan ID was not found
    printf("Loan ID %d not found or not assigned to you.\n", loanToProcess);
    fclose(loanFile);
}
void approveOrRejectLoanApp(const char *username) {
    Loan loanRecord;
    char employeeId[30]; // To store the assigned employee's ID
    int found = 0; // To check if any loans are found
    int loanToProcess;

    // Step 1: Find the employee ID based on the username from the user database
    User userRecord;
    FILE *userFile = fopen(USER_DB_PATH, "rb");
    if (userFile == NULL) {
        printf("Error: Could not open users database.\n");
        return;
    }

    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0 && strcmp(userRecord.role, "Employee") == 0) {
            sprintf(employeeId, "%d", userRecord.id); // Store the ID as a string
            found = 1;
            break;
        }
    }
    fclose(userFile);

    if (!found) {
        printf("Error: Employee with username '%s' not found or is not an employee.\n", username);
        return;
    }

    // Step 2: Process loans assigned to this employee
    FILE *loanFile = fopen(LOAN_DB_PATH, "rb+");
    if (loanFile == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }

    // Display header for the table
    printf("Loans Assigned to Employee (ID: %s) for Approval/Rejection:\n", employeeId);
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    printf("| Loan ID |    Loan Amount  |   Status   |  Duration  |  Interest Rate  |  Username     |  User ID                |\n");
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");

    // Loop through loans and find ones with "Processing" status and assigned to this employee
    found = 0;  // Reset found flag
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (strcmp(loanRecord.assignedEmployeeId, employeeId) == 0 && strcmp(loanRecord.status, "Processing") == 0) {
            printf("| %7d | %15.2lf | %10s | %10d | %15.2f%% | %13s | %23s |\n",
                   loanRecord.loanId,
                   loanRecord.loanAmount,
                   loanRecord.status,
                   loanRecord.durationMonths,
                   loanRecord.interestRate,
                   loanRecord.username,
                   loanRecord.userId);
            found = 1;
        }
    }
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");

    if (!found) {
        printf("No loans in 'Processing' status assigned to you.\n");
        fclose(loanFile);
        return;
    }

    // Ask for the loan ID to approve or reject
    printf("Enter the Loan ID you want to process: ");
    scanf("%d", &loanToProcess);

    // Rewind the file pointer to start from the beginning
    rewind(loanFile);

    // Search for the loan by ID and assigned employee ID
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (loanRecord.loanId == loanToProcess && strcmp(loanRecord.assignedEmployeeId, employeeId) == 0) {
            int decision;
            printf("Enter 1 to Approve or 0 to Reject the loan (ID: %d): ", loanToProcess);
            scanf("%d", &decision);

            // Update loan status based on the decision
            if (decision == 1) {
                strcpy(loanRecord.status, "Approved");
                printf("Loan ID %d has been approved.\n", loanToProcess);
            } else if (decision == 0) {
                strcpy(loanRecord.status, "Rejected");
                printf("Loan ID %d has been rejected.\n", loanToProcess);
            } else {
                printf("Invalid input. Please enter 1 or 0.\n");
                fclose(loanFile);
                return;
            }

            // Move the file pointer back by one record to overwrite the current loan record
            fseek(loanFile, -sizeof(Loan), SEEK_CUR);
            fwrite(&loanRecord, sizeof(Loan), 1, loanFile);
            fflush(loanFile);
            fclose(loanFile);
            return;
        }
    }

    // If loan ID was not found
    printf("Loan ID %d not found or not assigned to you.\n", loanToProcess);
    fclose(loanFile);
}
void viewAssignedLoans(const char *username) {
    Loan loanRecord;
    char employeeId[30]; // To store the assigned employee's ID
    int found = 0; // To check if loans are found for this employee

    // Step 1: Find the employee ID based on the username from the user database
    User userRecord;
    FILE *userFile = fopen(USER_DB_PATH, "rb");
    if (userFile == NULL) {
        printf("Error: Could not open users database.\n");
        return;
    }

    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0 && strcmp(userRecord.role, "Employee") == 0) {
            sprintf(employeeId, "%d", userRecord.id); // Store the employee ID as a string
            found = 1;
            break;
        }
    }
    fclose(userFile);

    if (!found) {
        printf("Error: Employee with username '%s' not found or is not an employee.\n", username);
        return;
    }

    // Step 2: Open the loan database and display all loans assigned to this employee
    FILE *loanFile = fopen(LOAN_DB_PATH, "rb");
    if (loanFile == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }

    // Display header for the table
    printf("Loans Assigned to Employee (ID: %s):\n", employeeId);
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+----------------+\n");
    printf("| Loan ID |    Loan Amount  |   Status   |  Duration  |  Interest Rate  |  Username     |  User ID                | Assigned Emp ID |\n");
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+----------------+\n");

    // Loop through loans and display those assigned to this employee, regardless of status
    found = 0; // Reset the found flag
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (strcmp(loanRecord.assignedEmployeeId, employeeId) == 0) {
            printf("| %7d | %15.2lf | %10s | %10d | %15.2f%% | %13s | %23s | %14s |\n",
                   loanRecord.loanId,
                   loanRecord.loanAmount,
                   loanRecord.status,
                   loanRecord.durationMonths,
                   loanRecord.interestRate,
                   loanRecord.username,
                   loanRecord.userId,
                   loanRecord.assignedEmployeeId);
            found = 1;
        }
    }
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+----------------+\n");

    if (!found) {
        printf("No loans assigned to you.\n");
    }

    fclose(loanFile);
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
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-15s %-15s %-15s %-10s %s\n", 
        "Transaction ID", "Sender", "Receiver", "Type", "Amount", "Date/Time");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    // Read all transactions and filter by user ID
    while (fread(&transaction, sizeof(Transaction), 1, file)) {
        if (strcmp(transaction.tranUser, user.username) == 0) {
            foundTransactions = 1; 
            printf("%-15d %-15s %-15s %-15s %-10.2f %s\n", 
                transaction.transactionId, 
                transaction.sendUser, 
                transaction.recUser, 
                transaction.type, 
                transaction.amount, 
                transaction.dateTime);
        }
    }
    printf("----------------------------------------------------------------------------------------------------------------------\n");


    if (!foundTransactions) {
        printf("No transactions found for User %s.\n", username);
    }

    fclose(file);
}