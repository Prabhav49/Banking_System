// /include/banking.h
#ifndef BANKING_H
#define BANKING_H

#define PORT 8080
#define MAX 1024
#define SA struct sockaddr

// Function declarations
const char* authenticate(const char *username, const char *password);

#endif
