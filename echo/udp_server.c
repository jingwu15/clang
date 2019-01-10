#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/socket.h>
//#include <net/netinet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

/*
typedef struct {
    
}my_addr;
*/

int main(int argc, const char *argv[]) {
    // int socket(int domain, int type, int protocol);
    // domain: 选择适用于通信的协议族
    // type：选择套接字类型
    // protocol：自动选择对应协议 0自动档
    // man 7 ip查询
    int udp_socket = 0;
    struct sockaddr_in myaddr, peeraddr;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == udp_socket) {
        perror("fail to socket");
        _exit(1);
    }

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(50000);
    //htons == host to network short
    //功能：将本机字节序转换成网络字节序(2个字节 short)  port
    myaddr.sin_addr.s_addr = inet_addr("192.168.1.80");
    //inet_addr
    //功能：将IP地址字符串转换成对应的大端字节序ip地址

    //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
    // bind 绑定
    // 1. sockfd socket创建的文件描述符
    // 2. 要填写一个结构体的首地址  struct sockaddr *    ---> man 2 bind
    // 实际填充结构体struct sockaddr_in ---> man 7 ip
    // 3. 地址结构体长度
    if(-1 == bind(udp_socket, (struct sockaddr *)&myaddr, sizeof(myaddr))) {
        perror("fail to bind");
        _exit(1);
    }

    char buf[100];
    socklen_t len;
    len = sizeof(peeraddr);

    while(1) {
        bzero(buf, sizeof(buf));
        //bzero
        //将数组内的元素全部清0，如果是字符数组则清成'\0'
        //if(-1 == recvfrom(udp_socket, buf, sizeof(buf), 0, NULL, NULL)) {
        if(-1 == recvfrom(udp_socket, buf, sizeof(buf), 0, (struct sockaddr*)&peeraddr, &len)) {
            perror("fail to recvfrom");
            _exit(1);
        }

        //ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
        // 1. socket返回的文件描述符
        // 2. 用来接收数据的数组
        // 3. 数组长度
        // 4. flags 填0   默认自动档       是否阻塞
        // 5.对方的地址结构体信息
        // 6.对方的地址结构体长度的指针

        //sendto();
        printf("ip: %s port:%d buf: %s\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port), buf);
        //inet_ntoa ---> network to ascii
        //功能：将
    }

    return 0;
}

