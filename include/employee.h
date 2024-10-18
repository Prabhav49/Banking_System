#ifndef EMPLOYEE_H
#define EMPLOYEE_H

// Function declarations for customer operations
// void displayEmployeeMenu(const char *username);
void addNewCustomer();
void modifyCustomerDetail();
void processLoanApplication(const char *username);
void approveOrRejectLoanApp(const char *username);
void viewAssignedLoans(const char *username);
int generateUserId() ;
void viewCustTrans();
int isUsernameUnique(const char *username) ;
void addCust(const char *fullName, const char *username, const char *password, const char *role, double balance, int active) ;
<<<<<<< HEAD
void empCase(const char*username, int choice);
=======

>>>>>>> eb84e4dd2cdbdde4094cb23157fe8cbad42d2cda

#endif