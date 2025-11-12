#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    // setup destination (server) address
    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(2112)
    };
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("failed to create socket");
        return -1;
    }

    char buffer[1024] = {0};
    socklen_t serv_len = sizeof(serv_addr);

    // REPL (Read-Eval-Print Loop)
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        if (strcmp(buffer, "exit") == 0) break;

        // send message to server
        ssize_t sent = sendto(
            sock,
            buffer,
            strlen(buffer),
            0,
            (struct sockaddr*)&serv_addr,
            serv_len
        );
        if (sent < 0) {
            perror("sendto failed");
            continue;
        }

        // receive response from server
        ssize_t num_read = recvfrom(
            sock,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr*)&serv_addr,
            &serv_len
        );
        if (num_read > 0) {
            buffer[num_read] = '\0';
            printf("< %s\n", buffer);
        } else {
            perror("recvfrom failed");
        }
    }

    close(sock);
    return 0;
}
