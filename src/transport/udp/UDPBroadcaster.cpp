/*
 * UDPBroadcaster.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include "UDPBroadcaster.h"

namespace transport {

UDPBroadcaster::~UDPBroadcaster()
{

}

bool UDPBroadcaster::Init(std::uint32_t port)
{
    int iOptval = 1;

    if ((sockId = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        return false;
    }

    if (setsockopt(sockId, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &iOptval, sizeof(int)) < 0)
    {
        close(sockId);
        return false;
    }
    memset(&sockAddr, 0, sizeof(struct sockaddr_in));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
    sockAddr.sin_port = htons(port);
    return true;
}

bool UDPBroadcaster::SendOnBuffer(std::uint8_t* buffer, size_t size)
{
    auto& sizeOfMsg = size;
    auto sizeOfSend = sendto(sockId, buffer, sizeOfMsg, 0, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr));
    if (sizeOfSend == -1)
    {
        delete[] buffer;
        return false;
    }
    else if (sizeOfSend < sizeOfMsg)
    {
        sizeOfSend = sendto(sockId, buffer, sizeOfMsg, 0, (struct sockaddr*)&sockAddr, sizeof(struct sockaddr));
        if (sizeOfSend == -1 || sizeOfSend < sizeOfMsg)
        {
            delete[] buffer;
            return false;
        }
    }

    return true;
}

bool UDPBroadcaster::Send(std::shared_ptr<Message> msg)
{
    auto sizeOfMsg = Message::SIZE_OF_HEADER + msg->header.bodyLength;
    if (sizeOfMsg > DEFAULT_BUFFER_SIZE)
    {
        auto buffer = new std::uint8_t[sizeOfMsg];
        auto bodyLength = msg->header.bodyLength;
        msg->header.encode();
        memcpy(buffer, &msg->header, Message::SIZE_OF_HEADER);
        memcpy(buffer + Message::SIZE_OF_HEADER, msg->body, bodyLength);
        auto&& rlt = SendOnBuffer(buffer, sizeOfMsg);
        delete[] buffer;
        return rlt;
    }
    else
    {
        auto bodyLength = msg->header.bodyLength;
        msg->header.encode();
        memcpy(defaultBuffer, &msg->header, Message::SIZE_OF_HEADER);
        memcpy(defaultBuffer + Message::SIZE_OF_HEADER, msg->body, bodyLength);
        auto&& rlt = SendOnBuffer(defaultBuffer, sizeOfMsg);
        return rlt;
    }
}

}  // namespace transport

