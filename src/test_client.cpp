/*
 * test_client.cpp
 *
 *  Created on: Jul 23, 2021
 *      Author: ubuntu
 */

#include "dcp/ClientEndPoint.h"
#include <cstdio>
#include <glog/logging.h>

int main()
{
    dcp::ClientConfig config;
    config.broadcastPort = 8899;
    std::shared_ptr<transport::TCPConnection> keepAlive;
    config.funcConnected = [&](std::shared_ptr<transport::TCPConnection> conn){
        LOG(INFO) << "conntected.";
        keepAlive = conn;
    };
    config.funcDisconnected = [](std::shared_ptr<transport::TCPConnection> conn){
        LOG(INFO) << "disconntected.";
    };
    dcp::ClientEndPoint ep;
    ep.SetConfig(config);
    ep.Start();
    getchar();
    return 0;
}

