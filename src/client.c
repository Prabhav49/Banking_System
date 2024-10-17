#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

// Function to handle login input from client side
void login_prompt(int client_socket) {
    char username[1024];
    char password[1024];
    char buffer[1024];

    // Display and input username from client side
    printf("Enter Username: ");
    fgets(username, sizeof(username), stdin);
    
    // Remove newline character from input
    username[strcspn(username, "\n")] = 0;
    
    // Send username to server
    write(client_socket, username, strlen(username));

    // Display and input password from client side
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    
    // Remove newline character from input
    password[strcspn(password, "\n")] = 0;
    
    // Send password to server
    write(client_socket, password, strlen(password));

    // Receive and display the authentication message from the server
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        printf("Server disconnected.\n");
        return;
    }
    buffer[bytes_read] = '\0';
    printf("%s", buffer);  // Print the authentication result from the server
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket (IPv4, TCP)
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Define the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address (localhost)

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to the server failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to the server.\n");

    // Call login_prompt directly since we are not waiting for any server prompt
    login_prompt(client_socket);  // Handle username and password input

    close(client_socket);
    return 0;
}
