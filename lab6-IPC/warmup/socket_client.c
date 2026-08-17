#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "unix_socket"

int main() {
    int sockfd;
    struct sockaddr_un serv_addr;
    char buffer[256];

    // create a socket
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    //fill in server address
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);
    
    //input
    printf("Enter message to send :\n");
    fgets(buffer,255,stdin);

    //send message to server
    int n = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*) &serv_addr, sizeof(serv_addr));
    close(sockfd);
    
}