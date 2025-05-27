#pragma once

#include "RotPacket.h"

#include <cstdint>
#include <array>
#include <vector>
#include <unistd.h>

#define CRSF_SIZE 26
#define CRSF_PAYLOAD_SIZE 24
#define CRSF_CHANNELS_AMOUT 16
#define CRSF_RC_PACKET 0x16
#define CRSF_TX_ADDR 0xC8
#define CRSF_RX_ADDR 0xEC
#define CRSF_DISABLE_RXLOSS 0xC2

class CRSF {
public:
    static constexpr size_t LINK_STATISTICS_PACKET_SIZE = 14;
    static constexpr int LINK_STATISTICS_PACKET_FREQUENCY = 3;
    static const uint8_t LINK_STATISTICS_PACKET[LINK_STATISTICS_PACKET_SIZE];

    static constexpr int THROTTLE_INDEX = 2;
    static constexpr int YAW_INDEX = 3;
    static constexpr int PITCH_INDEX = 1;
    static constexpr int ROLL_INDEX = 0;
    static constexpr int ARM_INDEX = 4;

    CRSF();

    void set(RotPacket packet);

    std::array<int, CRSF_CHANNELS_AMOUT> get() const;
    
    const std::array<uint8_t, CRSF_SIZE> unpack();
private:
    uint8_t computeCRC(const uint8_t* data, size_t length);

    std::array<int, CRSF_CHANNELS_AMOUT> channels;
};