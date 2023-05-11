#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]){
    int serv_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_adr,clnt_adr;

    if(argc!= 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    // 创建UDP套接字，参数传递SOCK_DGRAM
    serv_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(serv_sock == -1)
        error_handling("UDP socker creation error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    // 分配IP地址和端口号
    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    while(1){
        clnt_adr_sz = sizeof(clnt_adr);
        // recvfrom函数调用获取数据传输端的地址，利用该地址将接收的数据逆向重传
        str_len = recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        message[str_len] = '\0';
        printf("message from client : %s\n",message);
        sendto(serv_sock,message,str_len,0,(struct sockaddr*)&clnt_adr,clnt_adr_sz);
    }
    close(serv_sock);
    return 0;

}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}