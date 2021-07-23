/*
 * test_client.cpp
 *
 *  Created on: Jul 23, 2021
 *      Author: ubuntu
 */
#include "dcp/ServerEndPoint.h"
#include <cstdio>
#include <glog/logging.h>

int main()
{
    dcp::ServerEndPoint server;
    dcp::ServerConfig config;
    config.srvPort = 30000;
    config.broadcastPort = 8899;
    config.broadcastRate = 1.0;
    config.netcardName = "enp1s0";
    config.maxConnection = 10;
    std::shared_ptr<transport::TCPConnection> keepAlive;
    config.funcConnected = [&](std::shared_ptr<transport::TCPConnection> conn){
        LOG(INFO) << "conntected.";
        conn->Start();
        keepAlive = conn;
    };
    config.funcDisconnected = [](std::shared_ptr<transport::TCPConnection> conn){
        LOG(INFO) << "disconntected.";
    };
    server.SetConfig(config);
    server.Start();
    getchar();
    return 0;
}

