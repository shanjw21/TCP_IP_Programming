#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#define BUF_SIZE 1024

void error_handing(char* message);

int main(int argc, char**argv){
    int sock;
    sock = socket(PF_INET, SOCK_DGRAM,0);
    if(sock == -1)
        error_handing("socket() error.");

    struct sockaddr_in serv_adr;
    socklen_t serv_len;
    serv_len = sizeof(serv_adr);
    memset(&serv_adr,0,serv_len);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr *)&serv_adr, serv_len) == -1)
        error_handing("connect error.");
    else
        printf("Registered...........");

    char message[BUF_SIZE];
    int str_len;
    while(1){
        fputs("Insert message(q to quit): ",stdout);
        fgets(message,sizeof(message),stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"q\n"))    break;
        write(sock,message,strlen(message));
        str_len = read(sock,message,strlen(message)-1);
        message[str_len] = '\0';
        printf("Message from server : %s",message);
    }
    close(sock);
    return 0;
}

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}