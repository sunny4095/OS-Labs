#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCK_PATH "a_socket"
int main() {
    int sockfd;
    char buffer[16];
    struct sockaddr_un server_addr, client_addr;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_PATH);
    bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    socklen_t n = sizeof(client_addr);
    while(1) {
        int bytes_recieved = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&n);
        if (bytes_recieved <= 2) break;
        printf("||%s||\n",buffer);
    }
    unlink(SOCK_PATH);
    
}