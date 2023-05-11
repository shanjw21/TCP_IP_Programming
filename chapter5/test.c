#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<fcntl.h>

#define BUF_SIZE 1024
#define M_PI 3.1415926

// 测试 sprintf()函数的使用
#include <math.h>
void test1(){
    char str[80];

    int n = sprintf(str, "pi 的值 = %f", M_PI);
    puts(str);
    strcat(str," ");
    puts(str);
    // printf("%d\n",n);
}

void test2(){
    int n, i;
    char temp[20];
    char message[BUF_SIZE];
    puts("请输入你要计算的数字个数:");
    scanf("%d", &n);
    sprintf(temp, "%d", n);
    puts(message);
    // strcat(temp, " ");
    puts(temp);
    strcat(message, temp);
    puts(message);
    for (i = 0; i < n; i++)
    {
        printf("请输入第 %d 个数字:", i + 1);
        scanf("%s", temp);
        strcat(temp, " ");
        strcat(message, temp);
    }
    puts("请输入你要进行的运算符(+,-,*):");
    scanf("%s", temp);
    strcat(message, temp);
    puts(message);
}

int main(int argc, char*argv[]){
    // test1();
    test2();

    return 0;
}