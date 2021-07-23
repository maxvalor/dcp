#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <iostream> /* cout */
#include <unistd.h>/* gethostname */
#include <netdb.h> /* struct hostent */
#include <arpa/inet.h> /* inet_ntop */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <arpa/inet.h>

#include <thread>


bool GetIp(std::string ifname, std::string& ip)
{
    int sockfd;
    struct ifreq ifr;

    strncpy(ifr.ifr_name,ifname.c_str(),IFNAMSIZ-1);

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(ioctl(sockfd,SIOCGIFADDR,&ifr) == -1)
    {
        printf("Error\n");
        return false;
    }

    struct sockaddr_in *myaddr;
    myaddr = (struct sockaddr_in*)&(ifr.ifr_addr);
    printf("IP: %s\n",inet_ntoa(myaddr->sin_addr));
    ip = std::string(inet_ntoa(myaddr->sin_addr));
    return true;
}

#define MAXLINE 4096
#define TCP_PORT 6666

int tcp_server()
{
    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    char  buff[4096];
    int  n;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT);
    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connfd);
    }
    close(listenfd);
}

int main(void)
{
    std::thread([](){
        tcp_server();
    }).detach();

    std::string hostName;
    std::string Ip;

    bool ret = GetIp("enp1s0", Ip);
    if (true == ret) {
        std::cout << "Ip: " << Ip << std::endl;
    }

    int iFd;
    int iSendbytes;
    int iOptval = 1;
    struct sockaddr_in Addr;
    if ((iFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket fail\n");
        return -1;
    }

    if (setsockopt(iFd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        printf("setsockopt failed!");
    }
    memset(&Addr, 0, sizeof(struct sockaddr_in));
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    Addr.sin_port = htons(8899);

    char* buffer;
    int port = TCP_PORT;
    size_t s =  sizeof(size_t) + Ip.size() + sizeof(port);
    size_t ip_s = Ip.size();
    buffer = new char[s];
    memcpy(buffer, &ip_s, sizeof(size_t));
    memcpy(buffer + sizeof(size_t), Ip.c_str(), Ip.size());
    memcpy(buffer + sizeof(size_t) + Ip.size(), &port, sizeof(port));

    while (1)
    {
        if ((iSendbytes = sendto(iFd, buffer, s, 0, (struct sockaddr*)&Addr, sizeof(struct sockaddr))) == -1)
        {
            printf("sendto fail, errno=%d\n", errno);
            return -1;
        }
        printf("ip=%s, port=%d, iSendbytes=%d\n", Ip.c_str(), port, iSendbytes);
        sleep(1);
    }
    close(iFd);

    return 0;
}



