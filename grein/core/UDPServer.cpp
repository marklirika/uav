#include "UDPServer.h"
#include "RotPacket.h"

#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdint.h>
#include <array>

UDPServer::UDPServer(int port) {
    /* Init UDP Server */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

UDPServer::~UDPServer() {
    close(sockfd);
}

RotPacket UDPServer::receiveRotPacket() {
    std::array<uint8_t, RotPacket::PACKET_SIZE> buffer;
    socklen_t len = sizeof(cliaddr);

    int nbytes = recvfrom(sockfd, buffer.data(), sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);     
 
    if (nbytes < RotPacket::PACKET_SIZE) {
        std::cout << "Received invalid packet\nbytes";
        throw std::runtime_error("Failed to receive Rot packet");
    }

    return RotPacket::pack(buffer);
}