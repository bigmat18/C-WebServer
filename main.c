#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define QUEUE_SIZE 10
#define PORT 1234
#define check(status) if(status) {perror("Error"); exit(1);}


int main(void) {
    int server_socket;
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

    check(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0);
    
    listen(server_socket, QUEUE_SIZE);
    fprintf(stderr, "[+]Listening...\n");

    socklen_t addr_size;
    int client_socket;
    struct sockaddr_in client_address;
    char buffer[] = "HTTP/1.0 200 OK\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "<h>Hello from server!</h>\r\n";

    printf("%s", buffer);

    while(1){
        addr_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_size);
        fprintf(stderr, "[+]Client connected.\n");

        write(client_socket, buffer, strlen(buffer));

        close(client_socket);
        fprintf(stderr, "[+]Client disconneted.\n");

    }
 
    close(server_socket);

    return 0;
}
