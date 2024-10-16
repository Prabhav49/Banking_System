// transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

typedef struct {
    int transactionId;   // Unique transaction ID
    char tranUser[50];
    char sendUser[50];        // Sender's user ID
    char recUser[50];      // Receiver's user ID
    char type[20];        // Type of transaction (e.g., "Transfer")
    float amount;         // Amount of the transaction
    float currBalance;
    char dateTime[20];    // Date and time of transaction
} Transaction;

// Function declarations
int generateTransactionId(void);
void recordTransaction(const char *tranUser,const char *sendUser, const char *recUser, const char *type, float amount, float currBalance);
void viewTransactionHistory(const char *username);

#endif // TRANSACTION_H