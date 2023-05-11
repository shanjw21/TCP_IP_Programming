#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
void error_handling(char *message);

/*
    int inet_aton(const char* string, struct in_addr * addr);
    功能：将字符串形式的IP地址转换为32位整网络字节序数型，
    返回值：成功返回1，失败返回0
*/

int main(int argc,char * argv[]){

    char *addr1 = "127.232.124.79";
    struct sockaddr_in addr_inet;

    // 转换后的IP地址保存在sockaddr_in的in_addr型变量来才有意义，第二个参数要求得到in_addr变量地址值，可以省略保存IP地址信息的过程。
    if(!inet_aton(addr1,&addr_inet.sin_addr))
        error_handling("Conversion error");
    else    
        printf("Network ordered integer addr : %#x \n",addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}