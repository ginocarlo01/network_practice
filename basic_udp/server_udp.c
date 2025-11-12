#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>           
#include <arpa/inet.h>        
#include <netinet/in.h>       

int main() {
    
    struct sockaddr_in server_addr = {.sin_family = AF_INET,.sin_port = htons(2112)}, client_addr;
    socklen_t client_len = sizeof(client_addr);

    int server_sock = server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("failed to create socket");
        return 1;
    }

    char buffer[1024] = {0};

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_sock);
        return 1;
    }

    printf("UDP Server listening on 127.0.0.1:2112\n");

    while (1) {
        ssize_t bytes_read = recvfrom(server_sock,buffer,sizeof(buffer) - 1,0,(struct sockaddr*)&client_addr,&client_len);

        if (bytes_read < 0) {perror("recvfrom failed");continue;}

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("< %s\n", buffer);
        } else {
            perror("recvfrom failed");
        }
        
        ssize_t sent =  sendto(server_sock,buffer,bytes_read,0,(struct sockaddr*)&client_addr,client_len);
        if (sent < 0) {perror("sendto failed");continue;}

    }

    close(server_sock);
    return 0;
}
