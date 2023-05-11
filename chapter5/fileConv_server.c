#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
    收发文件的服务端/客户端：
    客户端接受用户输入的传输文件名，客户端请求服务的传输文件名所指文件，
    文件存在，服务的发送给客户端，反之断开连接

*/

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    // 创建socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // 绑定地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // 监听端口
    if (listen(server_fd, 10) == -1) {
        perror("listen");
        exit(1);
    }
    printf("Server listening on port %d...\n", port);

    // 接受客户端连接
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        exit(1);
    }
    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    char buffer[MAX_SIZE];
    char filename[MAX_SIZE];
    int len;

    // 接收文件名
    len = recv(client_fd, filename, MAX_SIZE, 0);
    if (len == -1) {
        perror("recv");
        exit(1);
    }
    filename[len] = '\0';
    printf("File name: %s\n", filename);

    // 打开文件并发送内容
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    // 将fp文件内容发送给客户端
    while ((len = fread(buffer, sizeof(char), MAX_SIZE, fp)) > 0){
        send(client_fd, buffer, len, 0);
    }

    // 关闭文件和socket
    fclose(fp);
    close(client_fd);
    close(server_fd);

    printf("File sent successfully.\n");

    return 0;
}
