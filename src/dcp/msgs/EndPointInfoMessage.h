/*
 * EndPointInfoMsg.h
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#pragma once

#include "../../transport/msg/Message.h"

namespace dcp {

class EndPointInfoMessage
{
public:
    // | SIZE_OF_IP | IP_STR | PORT | IN_USE |
    static const size_t SIZE_OF_MESSAGE_NECCESSARY =
            sizeof(size_t) + sizeof(std::uint32_t) + sizeof(bool);

    std::string ip;
    std::uint32_t port;
    bool inUse;

    EndPointInfoMessage() = default; // @suppress("Class members should be properly initialized")

    EndPointInfoMessage(std::shared_ptr<transport::Message> msg)
    {
        // INS has been taken
        msg->NextData(ip);
        msg->NextData(port);
        msg->NextData(inUse);
    }

    std::shared_ptr<transport::Message> toMessage()
    {
        auto msg = transport::Message::Generate(SIZE_OF_MESSAGE_NECCESSARY + ip.size());
        msg->AppendData(ip);
        msg->AppendData(port);
        msg->AppendData(inUse);
        return msg;
    }
};

}
