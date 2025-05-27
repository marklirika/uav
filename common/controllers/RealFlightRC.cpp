#include "RealFlightRC.h"

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <hidapi/hidapi.h>
#include <thread>
#include <chrono>
#include <array>

RealFlightRC::RealFlightRC() {
    if (hid_init()) {
        throw std::runtime_error("Error creating initializing HID API");
    }

    device = hid_open(vendor, product, nullptr);
    if (!device) {
        std::cerr << "Unable to open device VID: 0x" << std::hex << vendor
                    << " PID: 0x" << product << std::dec << std::endl;
        hid_exit();
        throw std::runtime_error("Error initializing HID device");
    }

    std::cout << "Device opened successfully. Reading data and stabilizing binary bit..." << std::endl;

    hid_set_nonblocking(device, 1);
}

RealFlightRC::~RealFlightRC() {
    hid_close(device);
    hid_exit();
}

void RealFlightRC::mapPacketValues(std::array<uint16_t, CTRL_INPUT_SIZE>& packet) {
    int input_min = 0;
    int input_max = 900;
    int output_min = 1000;
    int output_max = 2000;

    /* packet.size - 1 because we dont count ARM */
    for (size_t i = 0; i < packet.size() - 1; ++i) {
        
        float normalized_input = static_cast<float>(packet[i] - input_min) / (input_max - input_min);
        uint16_t mapped = static_cast<uint16_t>(output_min + normalized_input * (output_max - output_min));
        
        mapped += 75;

        if (i == YAW_INDEX) {
            mapped -= 5;
        }

        mapped = static_cast<uint16_t>((mapped + 5) / 10 * 10);

        if (i == THROTTLE_INDEX) {
            mapped -= 90;
        }

        if (i == ROLL_INDEX && mapped > 1100) {
            mapped -= 50;
        }

        if (mapped > 1900) {
            mapped = 1900;
        }

        packet[i] = mapped;
    }

    packet[ARM_INDEX] = packet[ARM_INDEX] ? 1600 : 900;
}

/** Original Mapping
 * Every BIT is covered with separate letter
 * Y - Yaw, T - Throttle, P - Pitch, R - Roll, A - Arm, n - Not Used
 * 1 byte: YYYY YYYY
 * 2 byte: PPPP PPPP
 * 3 byte: RRRR RRRR
 * 4 byte: YYTT PPRR
 * 5 byte: TTTT TTTT
 * 6 byte: nnnn nnnn
 * 7 byte: nnnn nnnn
 * 8 byte: nnnn nnAA
 */
std::array<uint16_t, IInput::CTRL_INPUT_SIZE> RealFlightRC::parse10BitCtrlData(const std::array<uint8_t, RAW_PACKET_SIZE>& buffer) {
    std::array<uint16_t, CTRL_INPUT_SIZE> packet;
    uint64_t current_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();

    /* Throttle (T) */
    packet[THROTTLE_INDEX] = static_cast<uint16_t>(buffer[4]) | (static_cast<uint16_t>((buffer[3] >> 4) & 0x03) << 8);

    /* Yaw (Y) */
    packet[YAW_INDEX] = static_cast<uint16_t>(buffer[0]) | (static_cast<uint16_t>((buffer[3] >> 6) & 0x03) << 8);

    /* Pitch (P) */
    packet[PITCH_INDEX] = static_cast<uint16_t>(buffer[1]) | (static_cast<uint16_t>((buffer[3] >> 2) & 0x03) << 8);

    /* Roll (R) */
    packet[ROLL_INDEX] = static_cast<uint16_t>(buffer[2]) | (static_cast<uint16_t>((buffer[3] >> 0) & 0x03) << 8);

    /* arm bit posirion is the last bit (SWC switch on RC)*/
    currentArmBit = (buffer[7] & 0x01);

    if (currentArmBit == lastArmBit) {
        if (stableArmTime == 0) {
            stableArmTime = current_time;
        } else if (current_time - stableArmTime >= STABILITY_THRESHOLD_MS) {
            stableArmState = currentArmBit;
        }
    } else {
        stableArmTime = 0;
        stableArmState = false;
    }
    lastArmBit = currentArmBit;

    packet[ARM_INDEX] = stableArmState;

    return packet;
}

void RealFlightRC::handleInput() {
    std::array<uint8_t, RAW_PACKET_SIZE> buffer{};
    int bytesRead = hid_read(device, buffer.data(), buffer.size());

    if (bytesRead == RAW_PACKET_SIZE) {
        channels = parse10BitCtrlData(buffer);

        mapPacketValues(channels);

        std::cout << "Packet (T, Y, P, R): "
                    << channels[0] << ", " << channels[1] << ", " << channels[2] << ", " << channels[3] << ", " << channels[4]
                    << ", Raw Arm Bit: " << currentArmBit
                    << ", Stable Arm State: " << stableArmState << std::endl;
    } else {
        if (bytesRead < 0) {
            std::cerr << "Read error or device disconnected: " << hid_error(device) << std::endl;
        }
    }
}