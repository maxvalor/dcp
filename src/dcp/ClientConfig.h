/*
 * ServerConfig.h
 *
 *  Created on: Jul 23, 2021
 *      Author: ubuntu
 */

#ifndef DCP_SERVERCONFIG_H_
#define DCP_SERVERCONFIG_H_

#include <string>
#include <functional>
#include "../transport/tcp/TCPConnection.h"

namespace dcp
{

struct ClientConfig
{
    std::uint32_t srvPort;
    std::uint32_t broadcastPort;
    std::function<void(std::shared_ptr<transport::TCPConnection>)> funcConnected;
    std::function<void(std::shared_ptr<transport::TCPConnection>)> funcDisconnected;
};

}

#endif /* DCP_SERVERCONFIG_H_ */
