// /src/login.c
#include <stdio.h>
#include <string.h>
#include "banking.h"

const char* authenticate(const char *username, const char *password) {
    // Predefined roles with credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        return "Admin Login Successful!";
    } else if (strcmp(username, "manager") == 0 && strcmp(password, "manager123") == 0) {
        return "Manager Login Successful!";
    } else if (strcmp(username, "employee") == 0 && strcmp(password, "employee123") == 0) {
        return "Employee Login Successful!";
    } else if (strcmp(username, "customer") == 0 && strcmp(password, "customer123") == 0) {
        return "Customer Login Successful!";
    } else {
        return "Login Failed!";
    }
}
