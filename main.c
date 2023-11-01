#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/_types/_null.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>

#define BUFFER_SIZE 4096
#define QUEUE_SIZE 10
#define PORT 8081
#define check(status) if(status) {fprintf(stderr, "%s == Linea: %d, File: %s\n", strerror(errno), __LINE__, __FILE__); exit(1);}

ssize_t writeN(int fd, void *ptr, size_t n);
ssize_t readN(int fd, void *ptr, size_t n);
void format_request(char* request);

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
    char response[] = "HTTP/1.0 200 OK\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "<h>Hello from server!</h>\r\n";

    char buffer[BUFFER_SIZE];


    while(1){
        addr_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_size);
        fprintf(stderr, "[+]Client connected.\n");
        
        read(client_socket, buffer, BUFFER_SIZE);
        format_request(buffer);
        writeN(client_socket, response, strlen(response));
      
        close(client_socket);
        fprintf(stderr, "[+]Client disconneted.\n");

    }
 
    close(server_socket);

    return 0;
}

ssize_t writeN(int fd, void *ptr, size_t n) {  
   size_t   nleft;
   ssize_t  nwritten;
 
   nleft = n;
   while (nleft > 0) {
     if((nwritten = write(fd, ptr, nleft)) < 0) {
        if (nleft == n) return -1;
        else break;
     } else if (nwritten == 0) break; 
     nleft -= nwritten;
     ptr   += nwritten;
   }
   return(n - nleft);
}

ssize_t readN(int fd, void *ptr, size_t n) {  
   size_t   nleft;
   ssize_t  nread;
 
   nleft = n;
   while (nleft > 0) {
     if((nread = read(fd, ptr, nleft)) < 0) {
        if (nleft == n) return -1;
        else break;
     } else if (nread == 0) break;
     nleft -= nread;
     ptr   += nread;
   }
   return(n - nleft);
}

void format_request(char* buffer) {
    char *token, *rest = NULL;
    buffer[BUFFER_SIZE] = '\0';

    for (token = strtok_r(buffer, "\r\n", &rest); 
         token != NULL;
         token = strtok_r(NULL, "\r\n", &rest)) {
        fprintf(stderr, "%s\n", token);
    }
}




