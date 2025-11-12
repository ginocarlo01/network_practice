#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>           
#include <arpa/inet.h>        
#include <netinet/in.h>       

int main() {
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("failed to create socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(2112);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        return 1;
    }

    printf("UDP Server listening on 127.0.0.1:2112\n");

    while (1) {
        ssize_t bytes_read = recvfrom(
            server_sock,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr*)&client_addr,
            &client_len
        );

        if (bytes_read < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[bytes_read] = '\0'; 
        printf("From %s:%d > %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        sendto(
            server_sock,
            buffer,
            bytes_read,
            0,
            (struct sockaddr*)&client_addr,
            client_len
        );
    }

    close(server_sock);
    return 0;
}
