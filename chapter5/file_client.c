#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFSIZE 1024

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFSIZE];
    int fd;

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname filename\n", argv[0]);
        exit(1);
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    // Send file name to server
    memset(buffer, 0, BUFSIZE);
    strncpy(buffer, argv[2], strlen(argv[2]));
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket");
    }

    // Open the file for writing
    fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        error("ERROR opening file");
    }

    // Receive file contents from server and write to local file
    while ((n = read(sockfd, buffer, BUFSIZE)) > 0) {
        if (write(fd, buffer, n) < 0) {
            error("ERROR writing to file");
        }
    }

    close(fd);
    close(sockfd);
    return 0;
}
