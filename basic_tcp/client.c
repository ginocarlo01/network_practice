#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_port = htons(2112) };
    inet_pton( AF_INET, "127.0.0.1", &serv_addr.sin_addr );

    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock < 0 ) { perror( "failed to create socket" ); return -1; }
    // this is what defines the TCP
    if ( connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) < 0 ) {
        perror( "failed to connect to server" ); return -1;
    }

    char buffer[1024] = {0};
    while (1) {
        printf("> ");
        fgets( buffer, sizeof(buffer), stdin );
        buffer[ strcspn( buffer, "\n" ) ] = 0; 

        if ( strcmp( buffer, "exit" ) == 0 ) { break; }

        send(sock, buffer, strlen(buffer), 0 );

        ssize_t bytes_read = recv(sock, buffer, sizeof(buffer) - 1, 0); // -1 para '\0'
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("< %s\n", buffer);
        }
    }

    close(sock);
    return 0;
}

