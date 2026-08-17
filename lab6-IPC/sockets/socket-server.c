#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#define SOCK_PATH "unix_socket"
int main() {
    int sockfd;
    char buffer[256];
    struct sockaddr_un serv_addr, cli_addr;

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    bzero(buffer,256);
    int len = sizeof(cli_addr);
    int bytes_recieved;

    while(1) {
        bytes_recieved = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &len);
        if (strlen(buffer) == 0) break;
        printf("%s",buffer);
        bzero(buffer,256);
    }
    unlink(SOCK_PATH);
}