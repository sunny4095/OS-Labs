#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>

#define SOCK_PATH "unix_socket"
#define SIZE 256
int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_un serv_addr;
    char buffer[SIZE];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);

    char *file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    bzero(buffer, SIZE);

    int bytes_read = read(fd,buffer,SIZE-1);
    int n;
    while (bytes_read > 0) {
        n = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
        bzero(buffer,SIZE);
        bytes_read = read(fd,buffer,SIZE-1);
    }
    bzero(buffer, SIZE);
    n = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    close(sockfd);
}