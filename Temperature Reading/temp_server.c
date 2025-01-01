#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 8080
#define BUFLEN 256

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFLEN];
    srand(time(NULL));  // Seed for random number generation

    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, 1) == -1) {
        perror("Listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections...\n");

    // Accept client connection
    if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        perror("Accept failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    // Continuously send random temperatures to the client
    while (1) {
        int temperature = rand() % 21 + 15; // Random temp between 15 and 35
        snprintf(buffer, sizeof(buffer), "Temperature: %d°C", temperature);

        if (temperature > 30) {
            strcat(buffer, " - Warning: High temperature!");
        }

        send(client_sock, buffer, strlen(buffer), 0);
        sleep(1);  // Delay for 1 second
    }

    close(client_sock);
    close(server_sock);
    return 0;
}

