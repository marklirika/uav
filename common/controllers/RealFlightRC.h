#pragma once

#include "IInput.h"
#include <hidapi/hidapi.h>

class RealFlightRC : public IInput {
public:
    static constexpr unsigned short vendor = 0x0b9b;
    static constexpr unsigned short product = 0x4012;

    /* This needed for reading non-stable analog values from device (they are constantly changing and are "undefined" in some sense) */
    static constexpr uint64_t STABILITY_THRESHOLD_MS = 20;

    /* 8 bytes, 10 bit for 1 ctrl value */
    static constexpr int RAW_PACKET_SIZE = 8;

    RealFlightRC();
    ~RealFlightRC();

    void handleInput() override;
    
private:
    void mapPacketValues(std::array<uint16_t, CTRL_INPUT_SIZE>& packet);
    std::array<uint16_t, CTRL_INPUT_SIZE> parse10BitCtrlData(const std::array<uint8_t, RAW_PACKET_SIZE>& buffer);
    
    bool stableArmState {false};
    bool currentArmBit {false};
    bool lastArmBit {false};
    uint64_t stableArmTime {0};
    hid_device* device;
};