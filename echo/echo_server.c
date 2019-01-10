/**
 * 多路复用(epoll)建立TCP连接
 * 多路复用涉及到三个函数：epoll_create, epoll_ctl, epoll_wait
 *
 * 编译：gcc -Wall tcp_epoll_server.c -o tcp_epoll_server
 *
 * 测试：
 * nc 192.168.1.107 50000
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

int main(int argc, const char *argv[]) {
    char buf[128] = {0};
    int tcp_socket, ret_bind, ret_listen, ret_recv, i, epfd, ret_epoll, ret_accept, lenEpoll;
    socklen_t lenServerAddr, lenClientAddr;
    struct sockaddr_in serverAddr, clientAddr;
    lenEpoll = 10;
    struct epoll_event epe, epes[lenEpoll];

    //建立tcp 连接
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket == -1) {
        perror("fail to socket");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(50000);
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    lenServerAddr = sizeof(serverAddr);
    ret_bind = bind(tcp_socket, (struct sockaddr *)&serverAddr, lenServerAddr);
    if(ret_bind == -1) {
        perror("fail to bind");
        exit(1);
    }
    ret_listen = listen(tcp_socket, 5);
    if(ret_listen == -1) {
        perror("fail to listen");
        exit(1);
    }

    //建立epoll
    epfd = epoll_create(lenEpoll);

    epe.events = EPOLLIN;
    epe.data.fd = tcp_socket;
    ret_epoll = epoll_ctl(epfd, EPOLL_CTL_ADD, tcp_socket, &epe);
    if(ret_epoll == -1) {
        perror("fail to epoll_ctl tcp_socket");
        exit(1);
    }

    while(1) {
        ret_epoll = epoll_wait(epfd, epes, lenEpoll, -1);
        if(ret_epoll == -1) {
            perror("fail to epoll_wait");
            exit(1);
        } else if(ret_epoll == 0) {
            printf("timeout\n");
        } else {
            for(i = 0; i < ret_epoll; i++) {
                if(epes[i].data.fd == tcp_socket) {
                    //添加accept 到epoll
                    ret_accept = accept(epes[i].data.fd, (struct sockaddr *)&clientAddr, &lenClientAddr);
                    if(ret_accept == -1) {
                        perror("fail to accept");
                        exit(1);
                    }
                    epe.events = EPOLLIN;
                    epe.data.fd = ret_accept;
                    ret_epoll = epoll_ctl(epfd, EPOLL_CTL_ADD, ret_accept, &epe);
                    if(ret_epoll == -1) {
                        perror("fail to epoll_ctl");
                        exit(1);
                    }
                } else {
                    //accept
                    bzero(buf, sizeof(buf));
                    ret_recv = recvfrom(epes[i].data.fd, buf, sizeof(buf), 0, (struct sockaddr *)&clientAddr, &lenClientAddr);
                    if(ret_recv == -1) {
                        perror("fail to rece");
                        exit(1);
                    } else if(ret_recv == 0) {
                        //连接结束
                        printf("connect close");
                        ret_epoll = epoll_ctl(epfd, EPOLL_CTL_DEL, epes[i].data.fd, NULL);
                        if(ret_epoll == -1) {
                            perror("fail to epoll_ctl del");
                            exit(1);
                        }
                        close(epes[i].data.fd);
                    } else {
                        printf("recv: %s", buf);
                    }
                }
            }
        }
    }
}
