#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char* message);

int main(int argc,char*argv[]){
    int sock;
    // 为收发数据准备的内存空间，需要数据累积到一定程度后再收发，所以通过数组创建。
    char opmsg[BUF_SIZE];

    int result, opnd_cnt,i;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    else    puts("connected......");

    fputs("Operand count : ", stdout);
    // 从用户输入中得到待算个数后，保存到数组opmsg，强制转换为char类型，协议中使用1个字节整数型传递。
    scanf("%d",&opnd_cnt);
    opmsg[0] = (char) opnd_cnt;

    for(i = 0; i < opnd_cnt; i++){
        printf("Operand %d : ", i+1);
        // 从用户处得到待算整数保存到数组opmsg，4字节int型数据要保存到char数组，因而转换成int指针类型。
        scanf("%d",(int*)&opmsg[i*OPSZ + 1]);
    }
    fgetc(stdin);
    fputs("Operator: ",stdout);
    scanf("%c",&opmsg[opnd_cnt*OPSZ + 1]);
    // 调用write函数一次性传输opmsg数组中的运算相关信息。 opmsg中保存一个字节的数字总数，opnd_cnt个4字节的数字，以及一个字节的运算符。
    write(sock,opmsg,opnd_cnt *OPSZ + 2);
    // 保存服务器端传输的运算结果，待接收的数据长度为4字节，调用1次read函数即可接收。
    read(sock,&result,RLT_SIZE);

    printf("Operation result: %d \n",result);
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
