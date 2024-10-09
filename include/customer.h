// /include/customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

// Function declarations for customer operations
void displayCustomerMenu(const char *username);
void viewAccountBalance(const char *username);
void depositMoney(const char *username);
void withdrawMoney(const char *username);
void transferFunds(const char *username);
void applyForLoan(const char *username);
void changePassword(const char *username);
void addFeedback(const char *username);
void viewTransactionHistory(const char *username);
void logout();
void userInfo(const char *username); 
void checkLoanStatus(const char *username);


#endif