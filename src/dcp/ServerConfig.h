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

struct ServerConfig
{
    std::string netcardName;
    std::uint32_t srvPort;
    std::uint32_t broadcastPort;
    double broadcastRate;
    std::function<void(std::shared_ptr<transport::TCPConnection>)> funcConnected;
    std::function<void(std::shared_ptr<transport::TCPConnection>)> funcDisconnected;
    size_t maxConnection;
};

}

#endif /* DCP_SERVERCONFIG_H_ */
