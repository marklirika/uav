#pragma once

#include "RotPacket.h"

#include <string>
#include <arpa/inet.h>

class UDPClient {
public:
    UDPClient() = delete;
    UDPClient(const std::string& serverIP, int serverPort);

    ~UDPClient();

    void sendRotPacket(RotPacket packet);
    
private:
    int sockfd;
    sockaddr_in servaddr;
};