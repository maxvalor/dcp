/*
 * IPFinder.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <arpa/inet.h>
#include "IPFinder.h"

namespace transport {

bool IPFinder::GetIP(std::string netcardName, std::string& ip)
{
    int sockfd;
    struct ifreq ifr;

    strncpy(ifr.ifr_name,netcardName.c_str(),IFNAMSIZ-1);

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(ioctl(sockfd,SIOCGIFADDR,&ifr) == -1)
    {
        printf("Error\n");
        return false;
    }

    struct sockaddr_in *myaddr;
    myaddr = (struct sockaddr_in*)&(ifr.ifr_addr);
    ip = std::string(inet_ntoa(myaddr->sin_addr));
    return true;
}

}  // namespace transport

