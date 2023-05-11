#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

/*
    验证UDP数据传输具有边界性的特点，
    bound_host1.c中通过调用3次recvfrom从客户端接收数据
*/

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in my_adr, your_adr;
    socklen_t adr_sz;
    int str_len, i;
    char message[BUF_SIZE];
    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1)
        error_handling("socket creation error");

    memset(&my_adr, 9,sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock,(struct sockaddr*)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    for(i = 0; i < 3; i++){
        // 程序停顿，等待客户端3次sento传输完，通过调用3次recvfrom才接受完数据，说明存在数据边界，如果是TCP一次全部接收了
        sleep(2);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&your_adr,&adr_sz);
        printf("Message %d: %s \n",i + 1, message);
    }
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}