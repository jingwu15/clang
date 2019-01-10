#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//nc -u -l 172.27.1.12 60000
//nc -u 172.27.1.12 50000
int main(int argc, const char *argv[])
{
    int tcp_socket, con_socket, ret_recv;
    struct sockaddr_in peeraddr;
    char buf[128] = {0};
    int len;
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_socket < 0) {
        perror("fail to socket");
        exit(1);
    }

    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons(50000);
    //peeraddr.sin_addr.s_addr = inet_addr("172.27.1.12");
    peeraddr.sin_addr.s_addr = inet_addr("192.168.1.101");

    if(connect(tcp_socket, (struct sockaddr*)&peeraddr, sizeof(peeraddr)) == -1) {
        perror("fail to connect");
        exit(1);
    }
    while(1) {
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        if(strncmp(buf, "quit", 4) == 0) {
            break;
        }
        send(tcp_socket, buf, sizeof(buf), 0);
    }
    close(tcp_socket);

    return 0;
}

