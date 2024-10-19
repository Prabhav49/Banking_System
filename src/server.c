#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "banking.h"
#include "customer.h"
#include "manager.h"
#include "admin.h"
#include "employee.h"

#include <stdint.h> // For uint32_t

#define PORT 8080
#define BACKLOG 5 


int recv_string(int client_socket, char *dest, size_t max_len) {
    uint32_t str_len;

    if (read(client_socket, &str_len, sizeof(str_len)) <= 0) {
        return -1; 
    }

    if (str_len >= max_len) {
        return -1;
    }
    int bytes_read = read(client_socket, dest, str_len);
    if (bytes_read <= 0) {
        return -1;
    }
    dest[bytes_read] = '\0'; 
    return 0;
}
void handle_client(int client_socket) {
    char username[1024];
    char password[1024];
    
    User users[MAX_USERS];
    int userCount = 0;
    loadUsers(users, &userCount);

    while (1) {
        // Receive username using the new method
        if (recv_string(client_socket, username, sizeof(username)) < 0) {
            printf("Client disconnected or error while receiving username.\n");
            close(client_socket);
            return;
        }
        printf("Client sent Username: %s\n", username);

        // Receive password using the new method
        if (recv_string(client_socket, password, sizeof(password)) < 0) {
            printf("Client disconnected or error while receiving password.\n");
            close(client_socket);
            return;
        }
        printf("Client sent Password: %s\n", password);

        // Authenticate the user
        const char* auth_msg = authenticate(username, password, users, userCount);
        write(client_socket, auth_msg, strlen(auth_msg));
        printf("Authentication result: %s\n", auth_msg);

        // If authentication is successful, send role
        if (strcmp(auth_msg, "Login Successful!\n") == 0) {
            const char* role = checkRole(username, users, userCount);
            write(client_socket, role, strlen(role));
            printf("User %s has role: %s\n", username, role);
        } else {
            // Client failed to log in; continue to the next iteration
            continue;
        }

        // Wait for logout status
        int statusReceived;
        if (read(client_socket, &statusReceived, sizeof(statusReceived)) < 0) {
            perror("Failed to read logout status from client");
            break;
        }

        bool loggedOut = (statusReceived != 0);
        if (loggedOut) {
            printf("Client has logged out successfully.\n");
            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
            continue;  
        } else {
            printf("Client is still logged in.\n");
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create a socket (IPv4, TCP)
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept any incoming connection

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    // Start listening for connections (up to BACKLOG clients in the queue)
    if (listen(server_socket, BACKLOG) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }
    
    printf("Server is listening on port %d...\n", PORT);

    // Infinite loop to accept and handle clients
    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Client connection failed");
            continue;
        }

        // Fork a new process to handle each client
        if (fork() == 0) {
            // Child process: handle client
            close(server_socket); // Close the listening socket in the child process
            handle_client(client_socket);
            exit(0); // Exit the child process when done
        } else {
            // Parent process: continue to accept new clients
            close(client_socket); // Close client socket in parent
        }
    }

    close(server_socket);
    return 0;
}
