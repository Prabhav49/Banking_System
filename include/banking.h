// /include/banking.h
#ifndef BANKING_H
#define BANKING_H

#define PORT 8080
#define MAX 1024
#define MAX_USERS 100 
#define SA struct sockaddr

typedef struct {
    int id;
    char fullName[100];
    char username[50];
    char password[50];
    char role[20];
    double balance;
    int active; // 1 for active, 0 for inactive
} User;

void loadUsers(User users[], int *userCount);
const char* authenticate(const char *username, const char *password, User users[], int userCount);
void addUser(const char *fullName, const char *username, const char *password, const char *role, double balance, int active); 
const char* checkRole(const char *username, User users[], int userCount);
#endif
