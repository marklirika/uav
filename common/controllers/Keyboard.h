#pragma once

#include "IInput.h"
#include <termios.h>

class Keyboard : public IInput {
public:
    Keyboard();
    ~Keyboard();

    void handleInput() override;

private:
    char getKeypress();
    void mapKeyToChannel(char key);

    struct termios oldt{}, newt{};
};