#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          
#include <arpa/inet.h>       
#include <netinet/in.h>      

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    //buffer of 1024 bytes (each char is a byte)
    char buffer[1024];

    //defines the endpoint for communication
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {perror("failed to create socket");return 1;}
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(2112);

    //associates IP with the socket
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, 5) < 0) {
        perror("listen failed");
        close(server_sock);
        return 1;
    }

    printf("Server listening on 127.0.0.1:2112\n");

    while (1) {
        //when a connection arrives, open a new socket to communicate with it
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }

        printf("New connection from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        while (1) {
            ssize_t bytes_read = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
                if (bytes_read <= 0) {
                    printf("No more data, closing connection\n");
                    break;
                }

                buffer[bytes_read] = '\0';
                printf("> %s\n", buffer);

            //returns the message that was received
            send(client_sock, buffer, bytes_read, 0);
        }

        close(client_sock);
    }

    close(server_sock);
    return 0;
}
