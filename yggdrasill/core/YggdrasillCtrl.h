#pragma once

#include "IInput.h"
#include "UDPClient.h"

#include <memory>

enum class InputDevice {
    Keyboard = 1,
    RealFlightRC = 2,
};

class YggdrasillCtrl {
public:
    YggdrasillCtrl() = delete;
    YggdrasillCtrl(const std::string& serverIP, int serverPort, InputDevice device);
    
    ~YggdrasillCtrl() {}

    void run();

private:
    UDPClient client;
    std::unique_ptr<IInput> inputDevice;
};