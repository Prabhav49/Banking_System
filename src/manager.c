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
#include "loan.h"
#include "manager.h"\

#define MAX_PASSWORD_LENGTH 50
#define USER_DB_PATH "../db/users.db"
#define LOAN_DB_PATH "../db/loans.db" 
