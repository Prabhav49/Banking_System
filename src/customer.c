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
#include "loan.h"

#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db"
#define MAX_USERS 100 
#define DB_PATH "../db/users.db"


void displayCustomerMenu(const char *username) {
    int choice;

    while (1) {
        printf("\nCustomer Menu:\n");
        printf("1. View Account Balance\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Transfer Funds\n");
        printf("5. Apply For a Loan\n");
        printf("6. Check Loan Status\n");
        printf("7. Add Feedback\n");
        printf("8. View Transaction History\n");
        printf("9. Change Password\n");
        printf("10. Logout\n");
        printf("11. Exit\n");
        printf("12. User Info\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAccountBalance(username);
                break;
            case 2:
                depositMoney(username);
                break;
            case 3:
                withdrawMoney(username);
                break;
            case 4:
                transferFunds(username);
                break;
            case 5:
                applyForLoan(username);
                break;
            case 6:
                checkLoanStatus(username);
                break;
            case 7:
                addFeedback(username);
                break;
            case 8:
                viewTransactionHistory(username);
                break;
            case 9:
                changePassword(username);
                break;    
            case 10:
                exit(0);
                return; 
            case 11:
                exit(0);
            case 12:
                userInfo(username);  
                break;  
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

int generateTransactionId() {
    FILE *file = fopen("../db/transactions.db", "rb");
    if (!file) {
        return 1; 
    }
    
    Transaction transaction;
    int maxId = 0;

    while (fread(&transaction, sizeof(Transaction), 1, file)) {
        if (transaction.transactionId > maxId) {
            maxId = transaction.transactionId;
        }
    }

    fclose(file);
    return maxId + 1; 
}

void recordTransaction(const char *tranUser,const char *sendUser,const char *recUser, const char *type, float amount,float currBalance) {
    FILE *file = fopen("../db/transactions.db", "ab");
    if (!file) {
        perror("Failed to open transactions database");
        return;
    }

    Transaction transaction;
    transaction.transactionId = generateTransactionId();
    strcpy(transaction.tranUser , tranUser);
    strcpy(transaction.sendUser ,sendUser);
    strcpy(transaction.recUser ,recUser);
    strcpy(transaction.type, type);
    transaction.amount = amount;
    transaction.currBalance = currBalance;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(transaction.dateTime, sizeof(transaction.dateTime), "%Y-%m-%d %H:%M:%S", t);

    fwrite(&transaction, sizeof(Transaction), 1, file);
    fclose(file);
}



void viewAccountBalance(const char *username) {
    FILE *file = fopen("../db/users.db", "rb"); 
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User user;
    int found = 0;

    // Read each user from the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            if (user.active) { 
                printf("Balance: %.2f\n", user.balance);
            } else {
                printf("User account is inactive.\n");
            }
            found = 1;  
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("User not found.\n");
    }
}

void depositMoney(const char *username) {
    FILE *file = fopen("../db/users.db", "r+b");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User user;
    int found = 0;
    float depositAmount;

    // Ask the user for the deposit amount
    printf("Enter the amount to deposit: ");
    scanf("%f", &depositAmount);

    // Read each user from the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            if (user.active) {
                user.balance += depositAmount; 
                fseek(file, -sizeof(User), SEEK_CUR); 
                fwrite(&user, sizeof(User), 1, file); 
                printf("Deposited %.2f to %s's account. New balance: %.2f\n", depositAmount, user.username, user.balance);
                recordTransaction(user.username,user.username, user.username, "Deposit", depositAmount, user.balance); 
         
            } else {
                printf("User account is inactive. Cannot deposit money.\n");
            }
            found = 1;  
            break; 
        }
    }

    fclose(file);

    if (!found) {
        printf("User not found.\n");
    }
}

void withdrawMoney(const char *username) {
    FILE *file = fopen("../db/users.db", "r+b");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User user;
    int found = 0;
    float withdrawAmount;

    // Ask the user for the deposit amount
    printf("Enter the amount to Withdraw: ");
    scanf("%f", &withdrawAmount);

    // Read each user from the database
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.username, username) == 0) {
            if (user.active) {
                if(user.balance < withdrawAmount){
                    printf("Error: Withdrawal amount exceeds current balance. Transaction failed.\n");
                    found = 1;
                    break;
                   
                }
                user.balance -= withdrawAmount; 
                fseek(file, -sizeof(User), SEEK_CUR); 
                fwrite(&user, sizeof(User), 1, file); 
                printf("Withdrawed %.2f to %s's account. New balance: %.2f\n", withdrawAmount, user.username, user.balance);
                recordTransaction(user.username,user.username, user.username, "Withdrawal", withdrawAmount,user.balance); 
          
            } else {
                printf("User account is inactive. Cannot deposit money.\n");
            }
            found = 1;  
            break; 
        }
    }

    fclose(file);

    if (!found) {
        printf("User not found.\n");
    }
}

void transferFunds(const char *username) {
    FILE *file = fopen("../db/users.db", "r+b");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    User sender;
    User recipient;
    float transferAmount;
    char recipientUsername[50];
    int foundSender = 0;
    int foundRecipient = 0;

    // Ask the user for the transfer amount
    printf("Enter the amount to transfer: ");
    scanf("%f", &transferAmount);
    printf("Enter recipient username: ");
    scanf("%49s", recipientUsername); 

    // Read through the file to find the sender
    while (fread(&sender, sizeof(User), 1, file)) {
        if (strcmp(sender.username, username) == 0) {
            if (sender.active) {
                foundSender = 1; 
                if (sender.balance < transferAmount) {
                    printf("Error: Transfer amount exceeds current balance. Transaction failed.\n");
                    fclose(file);
                    return; 
                }
            } else {
                printf("User account is inactive. Cannot transfer money.\n");
                fclose(file);
                return;
            }
            break; // Exit loop once the sender is found
        }
    }

    // Check if sender was found
    if (!foundSender) {
        printf("Sender not found.\n");
        fclose(file);
        return;
    }

    // Reset file pointer to start searching for the recipient
    rewind(file);

    // Search for the recipient
    long recipientPos = -1; // To keep track of recipient's position
    while (fread(&recipient, sizeof(User), 1, file)) {
        if (strcmp(recipient.username, recipientUsername) == 0) {
            foundRecipient = 1;  
            recipientPos = ftell(file) - sizeof(User); // Save position for the recipient
            break; 
        }
    }

    fclose(file); // Close the file after reading

    // Check if recipient was found
    if (!foundRecipient) {
        printf("Recipient not found.\n");
        return;
    }

    // If both sender and recipient are valid, proceed with the transfer
    // Update balances
    sender.balance -= transferAmount; 
    recipient.balance += transferAmount; 

    // Reopen the file to write the updated balances
    file = fopen("../db/users.db", "r+b");
    if (!file) {
        perror("Failed to open database");
        return;
    }

    // Write back updated sender information
    fseek(file, -sizeof(User), SEEK_CUR); 
    fwrite(&sender, sizeof(User), 1, file); 

    // Update the recipient's information
    fseek(file, recipientPos, SEEK_SET); // Move to the recipient's position
    fwrite(&recipient, sizeof(User), 1, file); 

    fclose(file); // Close the file after writing

    printf("Transferred %.2f to %s's account. New balance: %.2f\n", transferAmount, recipient.username, sender.balance);
    recordTransaction(sender.username,sender.username, recipient.username, "Transfer", transferAmount, sender.balance); 
    recordTransaction(recipient.username,sender.username, recipient.username, "Transfer", transferAmount, recipient.balance); 
}



void viewTransactionHistory(const char *username) {
    FILE *file = fopen("../db/users.db", "rb");
    if (!file) {
        perror("Failed to open user database");
        return;
    }

    User user;
    int found = 0;

    // Find user ID from username
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
    printf("----------------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-15s %-15s %-15s %-10s %-10s %s\n", "Transaction ID", "Sender ID", "Receiver ID", "Type", "Amount","Balance", "Date/Time");
    printf("----------------------------------------------------------------------------------------------------------\n");

    // Read all transactions and filter by user ID
    while (fread(&transaction, sizeof(Transaction), 1, file)) {
        if (strcmp(transaction.tranUser, user.username) == 0) {
            foundTransactions = 1; 
            printf("%-15d  %-15s %-15s %-15s %.2f %.2f %s\n", 
                   transaction.transactionId,
                   transaction.sendUser, 
                   transaction.recUser, 
                   transaction.type, 
                   transaction.amount,
                   transaction.currBalance, 
                   transaction.dateTime);
                   
        }
    }

    if (!foundTransactions) {
        printf("No transactions found for User %s.\n", username);
    }

    fclose(file);
}

void applyForLoan(const char *username) {
    Loan newLoan;
    strcpy(newLoan.username, username);
    
    // Simulate user input for loan amount only
    printf("Enter loan amount: ");
    scanf("%lf", &newLoan.loanAmount);
    
    // Other fields will be handled by the manager later
    strcpy(newLoan.userId, "");              // To be filled by the manager
    newLoan.interestRate = 0.0;              // To be determined by the manager
    newLoan.durationMonths = 0;              // To be decided by the manager
    strcpy(newLoan.status, "Pending");       // Initial status is "Pending"
    strcpy(newLoan.assignedEmployeeId, "");  // To be assigned by the manager
    
    // Open loan database file in append mode
    FILE *file = fopen(LOAN_DB_PATH, "a");
    if (file == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }
    
    // Write the new loan details to the file
    fwrite(&newLoan, sizeof(Loan), 1, file);
    fclose(file);
    
    printf("Loan application submitted successfully. Status: Pending\n");
}

void checkLoanStatus(const char *username) {
    Loan loanRecord;
    int found = 0; 
    
    FILE *file = fopen(LOAN_DB_PATH, "r");
    if (file == NULL) {
        printf("Error: Could not open loan database.\n");
        return;
    }

     // Table header
    printf("+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    printf("|    Loan Amount  |   Status   |  Duration  |  Interest Rate  | Assigned Emp  |         User ID          |\n");
    printf("+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");

    // Search through all loan records for the given username
    while (fread(&loanRecord, sizeof(Loan), 1, file)) {
        if (strcmp(loanRecord.username, username) == 0) {
            printf("| %15.2lf | %10s | %10d | %15.2f%% | %13s | %23s |\n",
                   loanRecord.loanAmount,
                   loanRecord.status,
                   loanRecord.durationMonths,
                   loanRecord.interestRate,
                   loanRecord.assignedEmployeeId[0] == '\0' ? "N/A" : loanRecord.assignedEmployeeId, // Show N/A if not assigned
                   loanRecord.userId[0] == '\0' ? "N/A" : loanRecord.userId); // Show N/A if not assigned
            found = 1;
        }
    }

    // Table footer
    printf("+-----------------+------------+------------+-----------------+---------------+-------------------------+\n");
    
    
    fclose(file);
    
    if (!found) {
        printf("No loan application found for user: %s\n", username);
    }
}

void changePassword(const char *username) {
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



void addFeedback(const char *username) {
    // Logic for adding feedback (update database)
    printf("Add Feedback functionality not yet implemented.\n");
}

void userInfo(const char *username){
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
        printf("\n-------------------------------------------------\n");
        printf("User Information:\n");
        printf("ID: %d\n", user.id);
        printf("Name: %s\n", user.fullName);
        printf("Username: %s\n", user.username);
        printf("Role: %s\n", user.role);
        printf("Balance: %.2f\n", user.balance);
        printf("Account Status: %s\n", user.active ? "Active" : "Inactive");
        printf("-------------------------------------------------\n");
    } else {
        printf("User not found.\n");
    }
}

