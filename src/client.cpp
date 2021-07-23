#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define MAXLINE 4096

int tcp_client(std::string ip, int port)
{
    int   sockfd, n;
    char  recvline[4096], sendline[4096];
    struct sockaddr_in  servaddr;

    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n", ip.c_str());
        return 0;
    }

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("send msg to server: \n");
    fgets(sendline, 4096, stdin);
    if( send(sockfd, sendline, strlen(sendline), 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    close(sockfd);
}

int main(void) {

    unsigned int iAddrLength;
    char rgMessage[2014];
    int iOptval = 1;
    int iFd;
    struct sockaddr_in Addr;

    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket fail\n");
        return -1;
    }

    if (setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        printf("setsockopt failed!\n");
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons(8899);
    iAddrLength = sizeof(Addr);

    if (bind(iFd, (struct sockaddr *)&Addr, sizeof(Addr)) == -1)
    {
        printf("bind failed!\n");
    }

    while (1)
    {
        if (recvfrom(iFd, rgMessage, sizeof(rgMessage), 0, (struct sockaddr *)&Addr, &iAddrLength) == -1)
        {
            printf("recv failed!\n");
        }

        size_t ip_s;
        memcpy(&ip_s, rgMessage, sizeof(size_t));
        std::cout << "ip_s:" << ip_s << std::endl;
        char* ip = new char[ip_s];
        memcpy(ip, rgMessage + sizeof(size_t), ip_s);
        printf("recv:%s", ip);
        int port;
        memcpy(&port, rgMessage + sizeof(size_t) + ip_s, sizeof(port));
        printf(", %d\n", port);

        tcp_client(ip, port);

        delete[] ip;
    }

    close(iFd);

    return 0;
}
