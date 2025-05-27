#pragma once

#include "RotPacket.h"

#include <iostream>
#include <cstring>
#include <arpa/inet.h>

class UDPServer {
public:
    UDPServer() = delete;
    UDPServer(int port);
    ~UDPServer();

    RotPacket receiveRotPacket();

private:
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
};