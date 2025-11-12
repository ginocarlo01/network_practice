#include <arpa/inet.h>

int recv_full(int sock, void *buffer, size_t size) {
    size_t total = 0;
    while (total < size) {
        ssize_t bytes = recv(sock, (unsigned char*)buffer + total, size - total, 0);
        if (bytes <= 0) {
            return -1;
        }
        total += bytes;
    }
    return 0;
}

int send_full(int sock, const void *buffer, size_t size) {
    size_t total = 0;
    while (total < size) {
        ssize_t bytes = send(sock, (const unsigned char*)buffer + total, size - total, 0);
        if (bytes <= 0) {
            return -1;
        }
        total += bytes;
    }
    return 0;
}