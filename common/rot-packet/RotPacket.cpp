#include "RotPacket.h"

#include <array>
#include <cstdint>
#include <cstdlib>

RotPacket::RotPacket()
    : header{0}, throttle{0}, yaw{0}, pitch{0}, roll{0}, arm{0}, manual{false} {}

RotPacket::RotPacket(uint8_t header, std::array<uint16_t, IInput::CTRL_INPUT_SIZE> ctrl, bool manual)
    : header{header}, throttle{ctrl[0]}, yaw{ctrl[1]}, pitch{ctrl[2]}, roll{ctrl[3]}, arm{ctrl[4]}, manual{manual} {}

RotPacket::~RotPacket() {}

RotPacket RotPacket::pack(std::array<uint8_t, PACKET_SIZE> data) {
    RotPacket packet;
    packet.header   = data[0];

    packet.throttle = (data[1] << 8) | data[2];
    packet.yaw      = (data[3] << 8) | data[4];
    packet.pitch    = (data[5] << 8) | data[6];
    packet.roll     = (data[7] << 8) | data[8];

    packet.arm      = (data[9] << 8) | data[10];
    packet.manual   = data[11] != 0;

    return packet;
}

std::array<uint8_t, RotPacket::PACKET_SIZE> RotPacket::unpack() {
    std::array<uint8_t, PACKET_SIZE> data{};
    data[0] = header;

    data[1] = throttle >> 8;
    data[2] = throttle & 0xFF;

    data[3] = yaw >> 8;
    data[4] = yaw & 0xFF;

    data[5] = pitch >> 8;
    data[6] = pitch & 0xFF;

    data[7] = roll >> 8;
    data[8] = roll & 0xFF;
    
    data[9] = arm >> 8;
    data[10] = arm & 0xFF;

    data[11] = manual ? 1 : 0;

    return data;
}