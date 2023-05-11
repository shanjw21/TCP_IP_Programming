#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int serv_sock;
    struct sockaddr_in serv_adr;
    socklen_t serv_len;

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1){
        perror("socket()");
        exit(1);
    }
    memset(&serv_adr,0,sizeof serv_adr);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    serv_len = sizeof(serv_adr);
    if(connect(serv_sock,(struct sockaddr*)&serv_adr, serv_len) == -1){
        perror("connect()");
        exit(1);
    }
    else{
        puts("Connected..............");
    }
    char message[BUF_SIZE];
    int strlen;
    while(1){
        fputs("Iput message(Q to quit): ",stdout);
        fgets(message,BUF_SIZE,stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;
        strlen = write(serv_sock,message,BUF_SIZE);
    }
    close(serv_sock);
    return 0;
}