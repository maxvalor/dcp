/*
 * ServerEndPoint.cpp
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#include "ServerEndPoint.h"
#include "msgs/EndPointInfoMessage.h"
#include "../transport/dev/IPFinder.h"
#include <glog/logging.h>

namespace dcp {

ServerEndPoint::~ServerEndPoint()
{
    tcpServer->Stop();
    broadcastThread->Stop();
    delete broadcastThread;
}

void ServerEndPoint::Start()
{
    transport::IPFinder ipFinder;
    ipFinder.GetIP(config.netcardName, ip);
    tcpServer = std::make_shared<transport::TCPServer>(
            ip,
            config.srvPort,
            config.funcConnected,
            config.funcDisconnected,
            config.maxConnection
            );
    std::thread([this](){
        tcpServer->Start();
    }).detach();
    broadcaster.Init(config.broadcastPort);


    broadcastThread = new util::LoopThread(config.broadcastRate,
        [this]()
        {
            // send message
            EndPointInfoMessage msg;
            msg.ip = ip;
            msg.port = config.srvPort;
            broadcaster.Send(msg.toMessage());
            LOG(INFO) << "ip:" << ip << ", port:" << config.srvPort;
        }
    );
    broadcastThread->Start();
}

}  // namespace transport



