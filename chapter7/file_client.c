#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]){
    FILE* fp;
    int sd;
    struct sockaddr_in serv_adr; 
    char buf[BUF_SIZE];
    int read_cnt;
    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }
    // 创建新文件以保存服务器端传输的文件数据
    fp = fopen("receive.dat","wb");
    sd = socket(PF_INET, SOCK_STREAM,0);

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sd,(struct sockaddr*)&serv_adr, sizeof(serv_adr)))
        error_handling("Connect() error");
    else{
        puts("Connected .......");
    }
    // 接收数据保存到文件fp中，知道接收到EOF为止
    while((read_cnt = read(sd,buf,BUF_SIZE)) != 0)
        fwrite((void*)buf,1,read_cnt,fp);
    
    puts("Recevied file data");
    //向服务器端发送感谢信息。如果服务器端没有关闭输入流，则可以接收到这个消息
    write(sd,"Thank you",10);

    fclose(fp);
    close(sd);
    return 0;
}

void error_handling(const char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}