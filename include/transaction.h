// transaction.h
#ifndef TRANSACTION_H
#define TRANSACTION_H

typedef struct {
    int transactionId;    // Unique transaction ID
    int senderId;         // Sender's user ID
    int receiverId;       // Receiver's user ID
    char type[20];        // Type of transaction (e.g., "Transfer")
    float amount;         // Amount of the transaction
    char dateTime[20];    // Date and time of transaction
} Transaction;

// Function declarations
int generateTransactionId(void);
void recordTransaction(int senderId, int receiverId, const char *type, float amount);
void viewTransactionHistory(const char *username);

#endif // TRANSACTION_H
