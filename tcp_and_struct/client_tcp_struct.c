#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.h"
#include "types.h"

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(2112);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    Entity entity = { .x = 10, .y = 9 };
    printf("sending entity: x=%d, y=%d\n", entity.x, entity.y);

    if (send_full(sock, &entity, sizeof(Entity)) < 0) {
        perror("send");
        close(sock);
        return 1;
    }

    if (recv_full(sock, &entity, sizeof(Entity)) < 0) {
        perror("recv");
        close(sock);
        return 1;
    }

    printf("Received updated entity: x=%d, y=%d\n", entity.x, entity.y);

    close(sock);
    return 0;
}
