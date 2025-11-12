#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    // setup connection details
    struct sockaddr_in serv_addr = { .sin_family = AF_INET, .sin_port = htons(2112) };
    inet_pton( AF_INET, "127.0.0.1", &serv_addr.sin_addr );

    // create socket and connect
    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock < 0 ) { perror( "failed to create socket" ); return -1; }
    if ( connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) < 0 ) {
        perror( "failed to connect to server" ); return -1;
    }

    // repl
    char buffer[1024] = {0};
    while (1) {
        // get input from user
        printf("> ");
        fgets( buffer, sizeof(buffer), stdin );
        buffer[ strcspn( buffer, "\n" ) ] = 0; // strip newline

        if ( strcmp( buffer, "exit" ) == 0 ) { break; }

        send( sock, buffer, strlen(buffer), 0 );

        int num_read = recv( sock, buffer, sizeof(buffer), 0 );
        if (num_read > 0) { // only if succeeded and got positive bytes
              buffer[ num_read ] = '\0'; // null terminate
        printf( "< %s\n", buffer );
	  }
    }

    close(sock);
    return 0;
}

