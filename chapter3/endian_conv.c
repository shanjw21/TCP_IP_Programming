#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    // 保存2字节的数据
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr=0x12345678;
    unsigned long net_addr;

    // htons将host_port的数据转换为网络字节序
    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    // %#x是带格式输出, 效果为在输出前加0x.
    printf("Host ordered port : %#x \n",host_port);
    printf("Network ordered port : %#x \n",net_port);
    printf("Host ordered port : %#lx \n",host_addr);
    printf("Network ordered port : %#lx \n",net_addr);

    return 0;
}