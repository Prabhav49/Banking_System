#ifndef EMPLOYEE_H
#define EMPLOYEE_H

// Function declarations for customer operations
void displayEmployeeMenu(const char *username);
void addNewCustomer();
void modifyCustomerDetail();
void processLoanApplication(const char *username);
void approveOrRejectLoanApp(const char *username);
void viewAssignedLoans(const char *username);
void changeEmpPassword(const char *username);
void empInfo(const char *username); 
int generateUserId() ;
void viewCustTrans();


#endif