
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
#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db" 


// void displayManMenu(const char *username) {
//     int choice;

//     while (1) {
//         printf("\nManager Menu:\n");
//         printf("1. Activate/Deactivate Customer Account\n");
//         printf("2. Assign Loan Application to Employee\n");
//         printf("3. Review Customer Feedback\n");
//         printf("4. Change Password\n");
//         printf("5. Logout\n");
//         printf("6. Exit\n");
//         printf("7. User Info\n");
//         printf("Enter your choice: ");
//         scanf("%d", &choice);



//         switch (choice) {
//             case 1:
//                 activateOrDeactivate();
//                 break;
//             case 2:
//                 assignLoanToEmp();
//                 break;
//             case 3:
//                 reviewCustFeedBack();
//                 break;
//             case 4:
//                 changePassword(username);
//                 break;
//             case 5:
//                 logout(username);
//                 break;
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


void manCase(const char* username,int choice){
    printf(" \n");
    printf("===========YOUR QUERY RESULT===========\n");
    printf("--------------------------------------------------------\n");
     switch (choice) {
            case 1:
                activateOrDeactivate();
                break;
            case 2:
                assignLoanToEmp();
                break;
            case 3:
                reviewCustFeedBack();
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

void activateOrDeactivate() {
    FILE *dbFile = fopen(USER_DB_PATH, "rb+");
    if (dbFile == NULL) {
        printf("Error: Could not open the database file.\n");
        return;
    }

    char targetUsername[50];
    printf("Enter the username of the account to activate/deactivate: ");
    scanf("%s", targetUsername);

    User user;
    int found = 0;

    // Search for the user by username
    while (fread(&user, sizeof(User), 1, dbFile)) {
        if (strcmp(user.username, targetUsername) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: User with username '%s' not found.\n", targetUsername);
        fclose(dbFile);
        return;
    }

    // Show current status and ask for activation/deactivation
    printf("Current status of user '%s': %s\n", user.username, user.active ? "Active" : "Inactive");
    printf("Enter 1 to activate or 0 to deactivate the account: ");
    int newStatus;
    scanf("%d", &newStatus);

    // Update the active status
    user.active = (newStatus == 1) ? 1 : 0;

    // Move file pointer back to the user's position to overwrite the record
    fseek(dbFile, -sizeof(User), SEEK_CUR);
    fwrite(&user, sizeof(User), 1, dbFile);

    printf("Account status for '%s' has been updated to: %s\n", user.username, user.active ? "Active" : "Inactive");

    fclose(dbFile);
}

void assignLoanToEmp() {
    Loan loanRecord;
    User userRecord;
    int foundLoan = 0, foundEmp = 0;
    int loanIdToAssign, employeeIdToAssign;
    FILE *loanFile, *userFile;
    
    // Step 1: List all loans with status "Pending"
    loanFile = fopen(LOAN_DB_PATH, "r+");  // Open loan file for both reading and writing
    if (loanFile == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }

    // Display pending loans in table format
    printf("Pending Loan Applications:\n");
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    printf("| Loan ID |    Loan Amount  |   Status   |  Duration  |  Interest Rate  | Assigned Emp  |         User ID          |\n");
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    
    // Search for all loans with "Pending" status
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (strcmp(loanRecord.status, "Pending") == 0) {
            printf("| %7d | %15.2lf | %10s | %10d | %15.2f%% | %13s | %23s |\n",
                   loanRecord.loanId,
                   loanRecord.loanAmount,
                   loanRecord.status,
                   loanRecord.durationMonths,
                   loanRecord.interestRate,
                   loanRecord.assignedEmployeeId[0] == '\0' ? "N/A" : loanRecord.assignedEmployeeId, // Show N/A if not assigned
                   loanRecord.userId[0] == '\0' ? "N/A" : loanRecord.userId);
            foundLoan = 1;
        }
    }
    
    // Table footer
    printf("+---------+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    
    if (!foundLoan) {
        printf("No pending loan applications.\n");
        fclose(loanFile);
        return;
    }

    // Step 2: Ask for loan ID to assign
    printf("Enter the Loan ID to assign: ");
    scanf("%d", &loanIdToAssign);

    // Step 3: List all employees
    userFile = fopen(USER_DB_PATH, "r");
    if (userFile == NULL) {
        printf("Error: Could not open user database.\n");
        fclose(loanFile);
        return;
    }

    printf("Available Employees:\n");
    printf("+---------+-------------------------+\n");
    printf("|  Emp ID |        Username         |\n");
    printf("+---------+-------------------------+\n");

    // Search through all users with role "Employee"
    while (fread(&userRecord, sizeof(User), 1, userFile)) {
        if (strcmp(userRecord.role, "Employee") == 0 && userRecord.active == 1) {
            printf("| %7d | %23s |\n", userRecord.id, userRecord.username);
            foundEmp = 1;
        }
    }
    
    printf("+---------+-------------------------+\n");

    if (!foundEmp) {
        printf("No active employees available.\n");
        fclose(userFile);
        fclose(loanFile);
        return;
    }

    // Step 4: Ask for employee ID to assign to the loan
    printf("Enter the Employee ID to assign the loan: ");
    scanf("%d", &employeeIdToAssign);
    
    // Step 5: Update the loan database with the assigned employee ID
    fseek(loanFile, 0, SEEK_SET);  // Reset file pointer to the beginning of the file
    while (fread(&loanRecord, sizeof(Loan), 1, loanFile)) {
        if (loanRecord.loanId == loanIdToAssign) {
            // Loan found, now assign the employee ID
            sprintf(loanRecord.assignedEmployeeId, "%d", employeeIdToAssign);
            sprintf(loanRecord.status,"%s","Processing");
            fseek(loanFile, -sizeof(Loan), SEEK_CUR);  // Move file pointer back to overwrite the record
            fwrite(&loanRecord, sizeof(Loan), 1, loanFile);  // Update the loan record
            printf("Loan ID %d has been assigned to Employee ID %d.\n", loanIdToAssign, employeeIdToAssign);
            break;
        }
    }

    // Close the files
    fclose(userFile);
    fclose(loanFile);
}

void reviewCustFeedBack() {
    Feedback feedbackRecord;
    int feedbackIdToProcess;
    int isResolved;
    int found = 0;  // To track if feedback is found

    // Open the feedback database file
    FILE *feedFile = fopen(FEEDBACK_DB_PATH, "rb+"); // Open for reading and writing
    if (feedFile == NULL) {
        printf("Error: Could not open feedback database.\n");
        return;
    }

    // Display all feedbacks in a line-by-line format
    printf("All Customer Feedbacks:\n\n");
    while (fread(&feedbackRecord, sizeof(Feedback), 1, feedFile)) {
        // Print each feedback with its details
        printf("----------------------------------------------\n" );
        printf("Feedback No: %d\n", feedbackRecord.feedNo);
        printf("Username: %s\n", feedbackRecord.username);
        printf("Feedback: %s\n", feedbackRecord.feedback);
        printf("Status: %s\n", feedbackRecord.status);
        printf("Feedback Time: %s\n", feedbackRecord.feedbackTime);
        printf("Status Update Time: %s\n\n", feedbackRecord.statusUpdateTime);
        printf("----------------------------------------------\n" );
    }

    // Ask the manager to enter the feedback ID to work on
    printf("Enter the Feedback No to process: ");
    scanf("%d", &feedbackIdToProcess);

    // Rewind the file to search for the feedback
    rewind(feedFile);
    while (fread(&feedbackRecord, sizeof(Feedback), 1, feedFile)) {
        if (feedbackRecord.feedNo == feedbackIdToProcess) {
            found = 1;

            // Update the status to "Working" if not already "Resolved"
            if (strcmp(feedbackRecord.status, "Resolved") != 0) {
                strcpy(feedbackRecord.status, "Working");

                // Update the status update timestamp to the current time
                time_t now = time(NULL);
                strftime(feedbackRecord.statusUpdateTime, sizeof(feedbackRecord.statusUpdateTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

                // Move the file pointer back by one record to overwrite the current feedback record
                fseek(feedFile, -sizeof(Feedback), SEEK_CUR);
                fwrite(&feedbackRecord, sizeof(Feedback), 1, feedFile);
                fflush(feedFile);

                printf("Feedback No %d status updated to: Working\n", feedbackIdToProcess);

                // Ask the manager whether the issue is resolved
                printf("Has the issue been resolved? Enter 1 for Resolved, 0 for Not Resolved: ");
                scanf("%d", &isResolved);

                if (isResolved == 1) {
                    strcpy(feedbackRecord.status, "Resolved");

                    // Update the status update timestamp again if resolved
                    time_t now = time(NULL);
                    strftime(feedbackRecord.statusUpdateTime, sizeof(feedbackRecord.statusUpdateTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

                    // Move the file pointer back again and update the record with the new status
                    fseek(feedFile, -sizeof(Feedback), SEEK_CUR);
                    fwrite(&feedbackRecord, sizeof(Feedback), 1, feedFile);
                    fflush(feedFile);

                    printf("Feedback No %d status updated to: Resolved\n", feedbackIdToProcess);
                } else {
                    printf("Feedback No %d remains in Working status.\n", feedbackIdToProcess);
                }
            } else {
                printf("Feedback No %d is already marked as Resolved.\n", feedbackIdToProcess);
            }

            fclose(feedFile);
            return;
        }
    }

    // If the feedback ID was not found
    if (!found) {
        printf("Error: Feedback No %d not found.\n", feedbackIdToProcess);
    }

    fclose(feedFile);
}
