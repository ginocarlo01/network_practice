#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "types.h"

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) { perror("socket"); return 1; }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(2112);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); close(server_sock); return 1;
    }

    if (listen(server_sock, 5) < 0) {
        perror("listen"); close(server_sock); return 1;
    }

    printf("server listening on 127.0.0.1:2112\n");

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) { perror("accept"); continue; }

        printf("Client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        Entity entity;

        if (recv_full(client_sock, &entity, sizeof(Entity)) < 0) {
            printf("Failed to receive entity.\n");
            close(client_sock);
            continue;
        }

        printf("Received entity: x=%d, y=%d\n", entity.x, entity.y);

        entity.x -= 1;
        entity.y -= 1;

        printf("modified entity: x=%d, y=%d\n", entity.x, entity.y);

        if (send_full(client_sock, &entity, sizeof(Entity)) < 0) {
            printf("Failed to send entity back.\n");
        }

        close(client_sock);
    }

    close(server_sock);
    return 0;
}
