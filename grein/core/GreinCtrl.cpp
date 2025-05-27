#include "GreinCtrl.h"

#include <thread>
#include <chrono>
#include <unistd.h>
#include <stdint.h>
#include <array>

GreinCtrl::GreinCtrl(int serverPort, const char* uartPort)
    : server{serverPort}, uart{uartPort}
{
    /* Opend periodic thread */
    std::thread periodicThread(&GreinCtrl::sendPeriodicMessage, this);
    
    periodicThread.detach();
}

GreinCtrl::~GreinCtrl() {}

void GreinCtrl::run() {
    while (true) {
        auto packet = server.receiveRotPacket();

        /* convert RotPacket into SBUS/CRSF */
        crsf.set(packet);

        auto data = crsf.unpack();
        uart.writeData(data.data(), data.size());

        /* Debug */
        auto channels = crsf.get();
        std::string response = "Key received: ";

        response += " | Mapped Channels: ";

        for (size_t i = 0; i < channels.size(); i++) {
            response += "ch" + std::to_string(i) + ":" + std::to_string(channels[i]) + " ";
        }

        std::cout << response << std::endl;
    }
}

void GreinCtrl::sendPeriodicMessage() {
    /* CRSF Packet for UART transmitting to FC (Flight Controller) */
    int periodCounter = 0;

    while (true) {
        auto packet = crsf.unpack();

        /* Link Statistics packet is REQUIRED by CRSF to make FC think network link is up and ready to work */
        if (periodCounter % CRSF::LINK_STATISTICS_PACKET_FREQUENCY == 0) {
            uart.writeData(CRSF::LINK_STATISTICS_PACKET, CRSF::LINK_STATISTICS_PACKET_SIZE);
        }
        else {
            /* Sending data to STM32F4xxx FC with SBUS RX set on */
            if (!packet.empty()) {
                uart.writeData(packet.data(), packet.size());
            }
        }

        periodCounter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}