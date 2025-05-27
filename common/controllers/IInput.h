#pragma once

#include <array>
#include <cstdint>

class IInput {
public:
    static constexpr int CTRL_INPUT_SIZE = 5;
    static constexpr int THROTTLE_INDEX = 0;
    static constexpr int YAW_INDEX = 1;
    static constexpr int PITCH_INDEX = 2;
    static constexpr int ROLL_INDEX = 3;
    static constexpr int ARM_INDEX = 4;

    virtual ~IInput() = default;

    std::array<uint16_t, CTRL_INPUT_SIZE> getChannels() { return channels; }

    virtual void handleInput() = 0;

protected:
    std::array<uint16_t, CTRL_INPUT_SIZE> channels;
};