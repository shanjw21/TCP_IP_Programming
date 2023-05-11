#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 1024

int main(int argc, char* argv[]){

    int serv_sock, clnt_sock;
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1){
        perror("socket()");
        exit(1);
    }

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t serv_len, clnt_len;
    memset(&serv_adr, 0, sizeof serv_adr);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    serv_len = sizeof(serv_adr);
    if(bind(serv_sock,(struct sockaddr *)&serv_adr,serv_len) == -1){
        perror("bind()");
        exit(1);
    }

    if(listen(serv_sock,10) == -1){
        perror("listen()");
        exit(1);
    }
    char buffer[BUF_SIZE];
    clnt_len = sizeof(clnt_adr);
    int strlen,cnt = 0;
    
    sleep(5);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_len);
    if(clnt_sock == -1){
        perror("accept()");
        exit(1);
    }
    for(int i = 0; i < 10; ++i){
        sleep(10);
        strlen = read(clnt_sock,buffer,BUF_SIZE);
        if(!strlen) break;
        buffer[strlen] = '\0';
        printf("message from client : %s",buffer);
        printf("read times : %d\n",cnt++);
    }
    
    close(serv_sock);
    close(clnt_sock);
    return 0;
}