#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    struct sockaddr_in serv_addr = {.sin_family = AF_INET,.sin_port = htons(2112)};
    socklen_t serv_len = sizeof(serv_addr);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("failed to create socket");
        return -1;
    }

    char buffer[1024] = {0};;
    

    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 

        if (strcmp(buffer, "exit") == 0) break;

        ssize_t sent = sendto(server_sock,buffer,strlen(buffer),0,(struct sockaddr*)&serv_addr,serv_len);
        if (sent < 0) {perror("sendto failed");continue;}

        ssize_t bytes_read = recvfrom(server_sock,buffer,sizeof(buffer) - 1,0,(struct sockaddr*)&serv_addr,&serv_len);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("< %s\n", buffer);
        } else {
            perror("recvfrom failed");
        }
    }

    close(server_sock);
    return 0;
}
