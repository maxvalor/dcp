/*
 * UDPBroadcastAudience.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <glog/logging.h>
#include "UDPBroadcastAudience.h"

namespace transport {

bool UDPBroadcastAudience::Init(std::uint32_t port)
{

    int iOptval = 1;

    if ((sockId = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return false;
    }

    if (setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        return false;
    }
    memset(&sockAddr, 0, sizeof(struct sockaddr_in));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = INADDR_ANY;
    sockAddr.sin_port = htons(port);

    if (bind(sockId, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == -1)
    {
        return false;
    }

    return true;
}

bool UDPBroadcastAudience::Receive(std::shared_ptr<Message>& msg)
{
    unsigned int iAddrLength = sizeof(sockAddr);
    if (recvfrom(sockId, defaultBuffer, sizeof(defaultBuffer), 0, (struct sockaddr *)&sockAddr, &iAddrLength)
            < Message::SIZE_OF_HEADER)
    {
        return false;
    }

    Message::Header header;

    memcpy(&header, defaultBuffer, Message::SIZE_OF_HEADER);

    header.decode();
    LOG(INFO) << "body size:" << header.bodyLength;
    msg = Message::Generate(header.bodyLength);
    memcpy(msg->body, defaultBuffer + Message::SIZE_OF_HEADER, msg->header.bodyLength);

    return true;
}

}  // namespace transport

