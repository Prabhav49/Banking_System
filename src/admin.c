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
#include "manager.h"
#include "loan.h"
#include "customer.h"
#include "feed.h"
#include "admin.h"

#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db" 


// void displayAdminMenu(const char *username) {
//     int choice;

//     while (1) {
//         printf("\nAdministration Menu:\n");
//         printf("1. Add New Bank Employee\n");
//         printf("2. Modify Customer/Employee Details\n");
//         printf("3. Manage User Roles\n");
//         printf("4. Change Password\n");
//         printf("5. Logout\n");
//         printf("6. Exit\n");
//         printf("7. User Info\n");
//         printf("Enter your choice: ");
//         scanf("%d", &choice);



//         switch (choice) {
//             case 1:
//                 addNewEmp();
//                 break;
//             case 2:
//                 modifyCustEmpDetail();
//                 break;
//             case 3:
//                 manageUserRoles();
//                 break;
//             case 4:
//                 changePassword(username);
//                 break;
//             case 5:
//                 printf("Logging Out");
//                 exit(0);
//             case 6:
//                 exit(0); 
//             case 7:
//                 userInfo(username); 
//                 break;    
//             default:
//                 printf("Invalid choice! Please try again.\n");
//         }
//     }
// }

void adminCase(const char* username,int choice){
    printf(" \n");
    printf("===========YOUR QUERY RESULT===========\n");
    printf("--------------------------------------------------------\n");
    switch (choice) {
            case 1:
                addNewEmp();
                break;
            case 2:
                modifyCustEmpDetail();
                break;
            case 3:
                manageUserRoles();
                break;
            case 4:
                changePassword(username);
                break;
            case 5:
                logout(username);
                break;
            case 6:
                userInfo(username); 
                break;    
            default:
                printf("Invalid choice! Please try again.\n");
        }

        printf("--------------------------------------------------------\n");
}

void addNewEmp(){
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

    strcpy(role, "Employee");
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

void modifyCustEmpDetail() {
    User userRecord;
    char username[50];
    int found = 0;

    // Ask for the username whose details need to be modified
    printf("Enter the username of the customer/employee to modify: ");
    scanf("%s", username);

    FILE *userFile = fopen(USER_DB_PATH, "rb+");  // Open file for reading and writing
    if (userFile == NULL) {
        printf("Error: Could not open user database.\n");
        return;
    }

    // Loop through the user database to find the matching username
    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0) {
            if (strcmp(userRecord.role, "Admin") == 0) {
                printf("Error: You cannot modify the details of an Admin user.\n");
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


void manageUserRoles() {
    User userRecord;
    char username[50];
    int found = 0;
    int newRoleChoice;

    // Ask for the username whose role needs to be managed
    printf("Enter the username of the user to manage the role: ");
    scanf("%s", username);

    FILE *userFile = fopen(USER_DB_PATH, "rb+");  // Open the user database file for reading and writing
    if (userFile == NULL) {
        printf("Error: Could not open users database.\n");
        return;
    }

    // Search for the user in the database
    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.username, username) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: User with username '%s' not found.\n", username);
        fclose(userFile);
        return;
    }

    // If user is Admin, prevent role modification
    if (strcmp(userRecord.role, "Admin") == 0) {
        printf("The role of 'Admin' cannot be changed.\n");
        fclose(userFile);
        return;
    }

    // Display the current role
    printf("Current role of %s: %s\n", userRecord.username, userRecord.role);

    // Ask for the new role choice
    printf("Choose the new role:\n");
    printf("0: Customer\n");
    printf("1: Employee\n");
    printf("2: Manager\n");
    printf("3: Administration\n");
    printf("Enter your choice: ");
    scanf("%d", &newRoleChoice);

    // Set the role based on the choice
    switch (newRoleChoice) {
        case 0:
            strcpy(userRecord.role, "Customer");
            break;
        case 1:
            strcpy(userRecord.role, "Employee");
            break;
        case 2:
            strcpy(userRecord.role, "Manager");
            break;
        case 3:
            strcpy(userRecord.role,"Admin");    
            break;
        default:
            printf("Invalid choice. Operation aborted.\n");
            fclose(userFile);
            return;
    }

    // Move the file pointer back by one record to update the user's record
    fseek(userFile, -sizeof(User), SEEK_CUR);
    fwrite(&userRecord, sizeof(User), 1, userFile);
    fflush(userFile);

    printf("User role updated successfully.\n");

    fclose(userFile);
}