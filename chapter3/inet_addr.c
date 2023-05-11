#include <stdio.h>
#include <arpa/inet.h>

/*
    1、格式化输出：
        %:表示这是一个格式化输出控制符
        #:表示在输出时加上前缀0x
        l:表示参数是一个长整型数值long
        x:表示以十六进制形式输出参数
    2、inet_addr(const char* string):
       功能：将字符串形式的IP地址转换成32位整数型数据
       返回值：成功返回32位大端序整数型值，失败返回INADDR_NONE
*/

int main(int argc, char *argv[]){
    char *addr1 = "1.2.3.4";
    // 1个字节能表示的最大整数是255，addr2是错误的IP地址，利用这个验证inet_addr函数的错误检查能力
    char *addr2 = "1.2.3.256";

    // 通过运行结果验证函数的能力
    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE){
        printf("Error occured! \n");
    }
    else{
        printf("Network ordered integer addr : %#lx \n",conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE)
        printf("Error occured \n");
    else    printf("Network ordered integer addr : %#lx \n",conv_addr);

    return 0;
}