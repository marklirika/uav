#pragma once

#include "IInput.h"

#include <array>
#include <cstdint>
#include <cstdlib>

enum class Protocol {
    CRSF = 1,
    SBUS = 2,
    UNKNOWN = 0,
};

class RotPacket {
public:
    /* CTRL_INPUT_SIZE is uint16_t, and in packet we need uint8_t */
    static constexpr size_t PACKET_SIZE = 1 + 2 * IInput::CTRL_INPUT_SIZE + 1;

    RotPacket();
    RotPacket(uint8_t header, std::array<uint16_t, IInput::CTRL_INPUT_SIZE> ctrl, bool manual);
    
    ~RotPacket();

    static RotPacket pack(std::array<uint8_t, PACKET_SIZE>);

    std::array<uint8_t, PACKET_SIZE> unpack();

    uint8_t header;
    uint16_t throttle;
    uint16_t yaw;
    uint16_t pitch;
    uint16_t roll;
    uint16_t arm;
    bool manual;
};