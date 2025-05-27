#pragma once

#include <cstdint>
#include <unordered_map>
#include <array>
#include <vector>

#define SBUS_START_BYTE 0x0F
#define SBUS_DEFAULT_CONTROL_BYTE 0x00
#define SBUS_END_BYTE 0x00
#define SBUS_SIZE 25

class SBUS {
public:
    SBUS();
    void mapKeyToChannel(char key);
    std::vector<uint16_t> getChannels() const;
    
    std::array<uint8_t, SBUS_SIZE> unpack();
private:
    std::vector<uint16_t> channels;
    std::unordered_map<char, int> keyMap;
};