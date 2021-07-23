project(111)
set(CMAKE_CXX_FLAGS "-O2 -Wall -std=c++14 -Wl,--copy-dt-needed-entries ${ERIZO_CMAKE_CXX_FLAGS}")
add_executable(client 
    src/test_client.cpp
    src/transport/tcp/TCPConnection.cpp
    src/transport/tcp/TCPServer.cpp
    src/transport/tcp/TCPClient.cpp
    src/transport/udp/UDPBroadcastAudience.cpp
    src/transport/dev/IPFinder.cpp
    src/dcp/ClientEndPoint.cpp)
add_executable(server 
    src/test_server.cpp
    src/transport/tcp/TCPConnection.cpp
    src/transport/tcp/TCPServer.cpp
    src/transport/tcp/TCPClient.cpp
    src/transport/udp/UDPBroadcaster.cpp
    src/transport/dev/IPFinder.cpp
    src/dcp/ServerEndPoint.cpp
    )

target_link_libraries(server
  -lpthread
  -lboost_system
  glog
)

target_link_libraries(client
  -lpthread
  -lboost_system
  glog
)