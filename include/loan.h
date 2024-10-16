// loan.h
#ifndef LOAN_H
#define LOAN_H

#include <stdio.h>
#include <string.h>

#define LOAN_DB_PATH "../db/loans.db"  

typedef struct {
    int loanId;
    char username[30]; 
    char userId[30];               
    double loanAmount;              
    float interestRate;             
    int durationMonths;             
    char status[15];                 // Loan status: "Approved", "Pending", "Rejected", "Processing"
    char assignedEmployeeId[30];     
} Loan;

void applyForLoan(const char *username);

#endif // LOAN_H