/*
 * ServerEndPoint.h
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#ifndef DCP_SERVERENDPOINT_H_
#define DCP_SERVERENDPOINT_H_


#include "../transport/udp/UDPBroadcaster.h"
#include "../transport/tcp/TCPServer.h"
#include "../util/LoopThread.h"
#include "ServerConfig.h"

namespace dcp
{

class ServerEndPoint
{
public:
    ServerEndPoint() {}
    ~ServerEndPoint();
    inline void SetConfig(ServerConfig& config)
    {
        this->config = config;
    }
    void Start();
    void SetOnConntected();

private:
    transport::UDPBroadcaster broadcaster;
    std::shared_ptr<transport::TCPServer> tcpServer;
    util::LoopThread* broadcastThread;
    ServerConfig config;
    std::string ip;
};


}


#endif /* DCP_SERVERENDPOINT_H_ */
