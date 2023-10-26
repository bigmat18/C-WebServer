#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_endian.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define HOST "127.0.0.1"
#define PORT 6969
#define check(status) if(status) {fprintf(stderr, "%s == Linea: %d, File: %s\n", strerror(errno), __LINE__, __FILE__); exit(1);}

int main(void){

    int sock;
    struct sockaddr_in addr;
    char buffer[1024];

    check((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0);
  
    printf("[+]TCP server socket created.\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(HOST);
    memset(&addr.sin_zero, '\0', sizeof(addr.sin_zero));

    check(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0);
    printf("Connected to the server.\n");

    bzero(buffer, 1024);
    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
