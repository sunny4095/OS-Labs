#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#define SOCK_PATH "unix_socket_example"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    mkfifo("fifo",0666);
	if (argc < 2) {
		printf("Usage: %s <file name>\n", argv[0]);
        unlink("fifo");
        exit(1);
	}
    int fd_read = open("fifo",O_RDONLY);
    int pid;
    read(fd_read,&pid,sizeof(int));
    close(fd_read);
    unlink("fifo");

	char *file_name = argv[1];

    int sockfd, portno, n;

    struct sockaddr_un serv_addr;
    char buffer[256];

    /* create socket, get sockfd handle */
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* fill in server address */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH); 

    /* ask user for input */
	// print file to terminal
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		printf("Error: file not found\n");
		exit(1);
	}
	// char line[256];
	// while (fgets(line, sizeof(line), file)) {
	// 	printf("%s", line);
	// }
    int bytes_read = fread(buffer,sizeof(buffer),sizeof(char),file);
    while (bytes_read > 0) {
        sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
        bytes_read = fread(buffer,sizeof(buffer),sizeof(char),file);
    }
    kill(pid,SIGKILL);

	fclose(file);


    close(sockfd);
    return 0;
}
