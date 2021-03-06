/*
 * tcp_client.cpp
 *
 *  Created on: Mar 9, 2021
 *      Author: ubuntu
 */

#include <glog/logging.h>
#include "TCPClient.h"

namespace transport
{

TCPClient::TCPClient(
      std::string ip,
      int port,
      std::function<void(std::shared_ptr<TCPConnection>)> on_connected,
      std::function<void(std::shared_ptr<TCPConnection>)> on_disconnected
      )
{
    ep = tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
    DLOG(INFO) << "server ip:" << ep.address() << ", port:" << ep.port();
    sock = std::make_shared<tcp::socket>(ioContext);
    OnConnected = on_connected;
    OnDisconnected = on_disconnected;
}

void TCPClient::Run()  {
    boost::system::error_code ec;
    boost::asio::io_service::work work(ioContext);
    ioContext.run(ec);
}

void TCPClient::Connect()
{
    sock->connect(ep);
    tcp::no_delay option(true);
    sock->set_option(option);
    auto conn = std::make_shared<TCPConnection>(sock, OnDisconnected);
    conn->Start();
    contextThread = std::thread([this](){
        Run();
    });
    contextThread.detach();
    OnConnected(conn);
}

}
