/*
 * tcp_service.cpp
 *
 *  Created on: Mar 9, 2021
 *      Author: ubuntu
 */

#include <glog/logging.h>
#include "TCPServer.h"

namespace transport
{

TCPServer::TCPServer(
      std::string ip,
      int port,
      std::function<void(std::shared_ptr<TCPConnection>)> funcConnected,
      std::function<void(std::shared_ptr<TCPConnection>)> funcDisconnected,
      size_t maxConnection
      )
{
    currentConnections = 0;
    this->maxConnection = maxConnection;
    OnConnected = funcConnected;
    OnDisconnected = funcDisconnected;
    ep = tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
    DLOG(INFO) << "server ip:" << ep.address() << ", port:" << ep.port();

    acceptor = std::make_shared<tcp::acceptor>(
            ioContext, ep
    );
}

void TCPServer::Run()  {
    Accept();

    boost::system::error_code ec;
    boost::asio::io_service::work work(ioContext);
    ioContext.run(ec);
}

void TCPServer::Accept()
{
    auto sock = std::make_shared<tcp::socket>(ioContext);
    acceptor->async_accept(*sock, [this, sock](boost::system::error_code ec)
    {
        if (ec == boost::system::errc::success    && currentConnections < maxConnection)
        {
            tcp::no_delay option(true);
            sock->set_option(option);
            ++currentConnections;
            auto connection = std::make_shared<TCPConnection>(
                sock,
                [this](std::shared_ptr<TCPConnection> conn)
                {
                  --currentConnections;
                  OnDisconnected(conn);
                }
            );

            OnConnected(connection);
            DLOG(INFO) << "currentConnections:" << currentConnections;
        }

        Accept();
    });
}

}



