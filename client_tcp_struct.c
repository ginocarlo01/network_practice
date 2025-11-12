#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int x;
    int y;
} Entity;

int recv_full(int sock, void *buffer, size_t size) {
    size_t total = 0;
    while (total < size) {
        ssize_t bytes = recv(sock, (unsigned char*)buffer + total, size - total, 0);
        if (bytes <= 0) return -1;
        total += bytes;
    }
    return 0;
}

int send_full(int sock, const void *buffer, size_t size) {
    size_t total = 0;
    while (total < size) {
        ssize_t bytes = send(sock, (const unsigned char*)buffer + total, size - total, 0);
        if (bytes <= 0) return -1;
        total += bytes;
    }
    return 0;
}

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
