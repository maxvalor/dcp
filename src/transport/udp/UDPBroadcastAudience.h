/*
 * UDPBroadcastAudience.h
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#pragma once

#include <chrono>
#include "../msg/Message.h"

namespace transport {

class UDPBroadcastAudience
{
public:
    bool Init(std::uint32_t port);
    bool Receive(std::shared_ptr<Message>& msg);

private:
    static const size_t DEFAULT_BUFFER_SIZE = 1024;
    int sockId;
    struct sockaddr_in sockAddr;
    std::uint8_t defaultBuffer[DEFAULT_BUFFER_SIZE];
};

}  // namespace transport
