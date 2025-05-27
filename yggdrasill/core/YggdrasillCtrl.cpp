#include "YggdrasillCtrl.h"
#include "RotPacket.h"
#include "Keyboard.h"
#include "RealFlightRC.h"

#include <iostream>
#include <memory>

YggdrasillCtrl::YggdrasillCtrl(const std::string& serverIP, int serverPort, InputDevice device) 
    : client {serverIP, serverPort}
{
    switch(device) {
        case InputDevice::Keyboard:
            inputDevice = std::make_unique<Keyboard>();
            break;
        case InputDevice::RealFlightRC:
            inputDevice = std::make_unique<RealFlightRC>();
            break;
    }
}

void YggdrasillCtrl::run() {
    std::cout << "Start sending commands." << std::endl;

    while (true) {
        inputDevice->handleInput();
        auto channels = inputDevice->getChannels();
        RotPacket packet{static_cast<int>(Protocol::CRSF), channels, true};
        client.sendRotPacket(packet);
    }
}