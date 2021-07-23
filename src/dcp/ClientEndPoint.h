/*
 * ClientEndPoint.h
 *
 *  Created on: Jul 22, 2021
 *      Author: ubuntu
 */

#ifndef DCP_CLIENTENDPOINT_H_
#define DCP_CLIENTENDPOINT_H_

#include "../transport/udp/UDPBroadcastAudience.h"
#include "../transport/tcp/TCPClient.h"
#include "ClientConfig.h"

namespace dcp
{

class ClientEndPoint
{
public:
    ClientEndPoint();
    void Start();
    inline void SetConfig(ClientConfig& config)
    {
        this->config = config;
    }

private:
    transport::UDPBroadcastAudience audience;
    std::shared_ptr<transport::TCPClient> tcpClient;
    ClientConfig config;
    bool connected;
};


}



#endif /* DCP_CLIENTENDPOINT_H_ */
