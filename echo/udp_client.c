#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[]) {
    int udp_socket = 0;
    struct sockaddr_in peeraddr;

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_socket < 0) {
        perror("fail to socket");
        _exit(1);
    }
    //对于客户端来说，ip地址随时变化，不需要bind，但其实可以用
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(50000);
    peeraddr.sin_addr.s_addr = inet_addr("192.168.1.80");   //0.0.0,0

    char buf[100];
    while(1) {
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        if(-1 == sendto(udp_socket, buf, sizeof(buf), 0, (struct sockaddr *)&peeraddr, sizeof(peeraddr))) {
            perror("fail to send");
            _exit(1);
        }
        //ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
    }

    return 0;
}


