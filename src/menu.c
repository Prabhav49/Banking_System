#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to display the Customer menu
char* getCustomerMenu() {
    char *menu = (char*)malloc(1024 * sizeof(char));  // Allocate memory for the menu string
    if (menu == NULL) {
        perror("Failed to allocate memory for Customer Menu");
        exit(EXIT_FAILURE);
    }

    sprintf(menu, "\nCustomer Menu:\n"
                  "----------------------------------\n" 
                  "1. View Account Balance\n"
                  "2. Deposit Money\n"
                  "3. Withdraw Money\n"
                  "4. Transfer Funds\n"
                  "5. Apply For a Loan\n"
                  "6. Check Loan Status\n"
                  "7. Add Feedback\n"
                  "8. View Transaction History\n"
                  "9. Change Password\n"
                  "10. Check Feedback Status\n"
                  "11. Logout\n"
                  "12. User Info\n"
                  "----------------------------------\n" );


    return menu;  // Return the pointer to the menu
}

// Function to display the Employee menu
char* getEmployeeMenu() {
    char *menu = (char*)malloc(1024 * sizeof(char));  // Allocate memory for the menu string
    if (menu == NULL) {
        perror("Failed to allocate memory for Employee Menu");
        exit(EXIT_FAILURE);
    }

    sprintf(menu, "\nEmployee Menu:\n"
                  "----------------------------------\n" 
                  "1. Add New Customer\n"
                  "2. Modify Customer Detail\n"
                  "3. Process Loan Application\n"
                  "4. Approve/Reject Loans\n"
                  "5. View Assigned Loan Applications\n"
                  "6. View Customer Transaction\n"
                  "7. Change Password\n"
                  "8. Logout\n"
                  "9. User Info\n"
                  "----------------------------------\n" );

    return menu;  // Return the pointer to the menu
}

// Function to display the Manager menu
char* getManagerMenu() {
    char *menu = (char*)malloc(1024 * sizeof(char));  // Allocate memory for the menu string
    if (menu == NULL) {
        perror("Failed to allocate memory for Manager Menu");
        exit(EXIT_FAILURE);
    }

    sprintf(menu, "\nManager Menu:\n"
                  "----------------------------------\n" 
                  "1. Activate/Deactivate Customer Account\n"
                  "2. Assign Loan Application to Employee\n"
                  "3. Review Customer Feedback\n"
                  "4. Change Password\n"
                  "5. Logout\n"
                  "6. User Info\n"
                  "----------------------------------\n" );

    return menu;  // Return the pointer to the menu
}

// Function to display the Administration menu
char* getAdminMenu() {
    char *menu = (char*)malloc(1024 * sizeof(char));  // Allocate memory for the menu string
    if (menu == NULL) {
        perror("Failed to allocate memory for Administration Menu");
        exit(EXIT_FAILURE);
    }

    sprintf(menu, "\nAdministration Menu:\n"
                  "----------------------------------\n" 
                  "1. Add New Bank Employee\n"
                  "2. Modify Customer/Employee Details\n"
                  "3. Manage User Roles\n"
                  "4. Change Password\n"
                  "5. Logout\n"
                  "6. User Info\n"
                  "----------------------------------\n" );

    return menu;  // Return the pointer to the menu
}
