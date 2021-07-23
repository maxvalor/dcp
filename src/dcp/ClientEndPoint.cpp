/*
 * ClientEndPoint.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#include "ClientEndPoint.h"
#include "msgs/EndPointInfoMessage.h"
#include <glog/logging.h>

namespace dcp
{

ClientEndPoint::ClientEndPoint()
{
    connected = false;
}

void ClientEndPoint::Start()
{
    if (!connected)
    {
        audience.Init(config.broadcastPort);
        std::shared_ptr<transport::Message> msg;
        audience.Receive(msg);

        EndPointInfoMessage epInfo(msg);
        LOG(INFO) << "ip:" << epInfo.ip << ", port:" << epInfo.port;

        tcpClient = std::make_shared<transport::TCPClient>(
                epInfo.ip,
                epInfo.port,
                config.funcConnected,
                config.funcDisconnected);
        tcpClient->Connect();
        connected = true;
    }
}

}

