#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/*
    char* inet_ntoa(struct in_addr adr);
    功能：将网络字节序整数型IP地址转换为字符串类型
*/

int main(int argc, char *argv[]){
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);
    //向inet_ntoa函数传递结构体变量addr1中的IP地址信息并调用函数，返回字符串形式的IP地址
    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr,str_ptr);
    printf("Dotted-Decimal notation1: %s \n",str_ptr);

    // 再次调用inet_ntoa函数，返回的地址会覆盖新的IP地址字符串。
    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n",str_ptr);
    printf("Dotted-Decimal notation3: %s \n",str_arr);


    return 0;
}