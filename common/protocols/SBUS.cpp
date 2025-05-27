#include "SBUS.h"
#include <iostream>

SBUS::SBUS() {
    channels.resize(16, 1050); // Initialize all channels to neutral value

    keyMap['w'] = 0;
    keyMap['s'] = 0;
    keyMap['a'] = 1;
    keyMap['d'] = 1;
    keyMap['r'] = 2;
    keyMap['f'] = 2;
    keyMap['q'] = 3;
    keyMap['e'] = 3;
    keyMap['z'] = 4;
    keyMap['x'] = 4;
}

std::array<uint8_t, SBUS_SIZE> SBUS::unpack() {
    std::array<uint8_t, SBUS_SIZE> packet{};

    packet[0] = SBUS_START_BYTE;

    uint8_t* payload = &packet[1];
    uint32_t bitBuffer = 0;
    uint8_t bitCount = 0;
    int payloadIndex = 0;

    for (int i = 0; i < 16; i++) {
        bitBuffer |= (channels[i] & 0x07FF) << bitCount;
        bitCount += 11;

        while (bitCount >= 8) {
            payload[payloadIndex++] = bitBuffer & 0xFF;
            bitBuffer >>= 8;
            bitCount -= 8;
        }
    }

    if (bitCount > 0) {
        payload[payloadIndex++] = bitBuffer & 0xFF;
    }

    /* TODO: Research on parity, failsafe mode and frame loss activation */
    packet[23] = SBUS_DEFAULT_CONTROL_BYTE;
    packet[24] = SBUS_END_BYTE;

    return packet;
}

void SBUS::mapKeyToChannel(char key) {
    switch (key) {
        /* ROLL */
        case 'a': 
            channels[0] -= 10; 
            if (channels[0] < 1050) channels[0] = 1050;
            break;
        case 'd': 
            channels[0] += 10; 
            if (channels[0] > 1900) channels[0] = 1900;
            break;

        /* PITCH */
        case 'w': 
            channels[1] += 10; 
            if (channels[1] > 1900) channels[1] = 1900;
            break;
        case 's': 
            channels[1] -= 10; 
            if (channels[1] < 1050) channels[1] = 1050;
            break;

        /* THROTTLE */
        case 'r': 
            channels[2] -= 10;  
            if (channels[2] > 1900) channels[2] = 1900;
            break;
        case 'f': 
            channels[2] += 10;  
            if (channels[2] > 1900) channels[2] = 1900;
            break;

        /* YAW */
        case 'q': 
            channels[3] -= 10;  
            if (channels[3] < 1050) channels[3] = 1050;
            break;
        case 'e': 
            channels[3] += 10;  
            if (channels[3] > 1900) channels[3] = 1900;
            break;

        /* DISARM */
        case 'z': 
            channels[4] = 1200;
            break;
        /* ARM */
        case 'x': 
            channels[4] = 1700;
            break;
        /* HZ-SHO-POPALO */
        case 'c': 
            channels[2] = 120;  
            break;

        default:  
            std::cout << "Invalid key: " << key << std::endl; 
            return;
    }
}


std::vector<uint16_t> SBUS::getChannels() const {
    return channels;
}
