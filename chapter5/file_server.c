#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080
#define BUFSIZE 1024

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, n;
    socklen_t clilen;
    char buffer[BUFSIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int fd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept");
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

        memset(buffer, 0, BUFSIZE);

        // Receive file name from client
        n = read(newsockfd, buffer, BUFSIZE);
        if (n < 0) {
            error("ERROR reading from socket");
        }

        printf("Received file name: %s\n", buffer);

        // Open the file
        fd = open(buffer, O_RDONLY);
        if (fd < 0) {
            error("ERROR opening file");
        }

        // Send the file contents to client
        while ((n = read(fd, buffer, BUFSIZE)) > 0) {
            if (write(newsockfd, buffer, n) < 0) {
                error("ERROR writing to socket");
            }
        }

        close(fd);
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
