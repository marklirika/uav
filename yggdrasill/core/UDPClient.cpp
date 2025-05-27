#include "UDPClient.h"
#include <unistd.h>
#include <termios.h>
#include <iostream>

UDPClient::UDPClient(const std::string& serverIP, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIP.c_str(), &servaddr.sin_addr);
}

UDPClient::~UDPClient() {
    close(sockfd);
}

void UDPClient::sendRotPacket(RotPacket packet) {
    auto data = packet.unpack();

    sendto(sockfd, data.data(), data.size(), MSG_CONFIRM, 
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
}

