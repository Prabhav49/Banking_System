// /include/customer.h
#ifndef CUSTOMER_H
#define CUSTOMER_H

// Function declarations for customer operations
// void displayCustomerMenu(const char *username);
void viewAccountBalance(const char *username);
void depositMoney(const char *username);
void withdrawMoney(const char *username);
void transferFunds(const char *username);
void applyForLoan(const char *username);
void changePassword(const char *username);
void addFeedback(const char *username);
void viewTransactionHistory(const char *username);
void userInfo(const char *username); 
void checkLoanStatus(const char *username);
void checkFeedBackStatus(const char* username);
<<<<<<< HEAD
void customerCase(const char* username,int choice);
=======
>>>>>>> eb84e4dd2cdbdde4094cb23157fe8cbad42d2cda

#endif