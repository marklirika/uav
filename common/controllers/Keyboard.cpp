#include "Keyboard.h"

#include <iostream>
#include <unistd.h>

Keyboard::Keyboard() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

Keyboard::~Keyboard() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

char Keyboard::getKeypress() {
    return getchar();
}

void Keyboard::mapKeyToChannel(char key) {
    switch (key) {
    /* TROTTLE */
    case 'r':
        channels[THROTTLE_INDEX] += 10;
        break;
    case 'f':
        channels[THROTTLE_INDEX] -= 10;
        break;
    case 'c':
        channels[THROTTLE_INDEX] = 120;
        break;
    /* YAW */
    case 'q':
        channels[YAW_INDEX] -= 10;
        break;
    case 'e':
        channels[YAW_INDEX] += 10;
        break;
    /* PITCH */
    case 'w':
        channels[PITCH_INDEX] += 10;
        break;
    case 's':
        channels[PITCH_INDEX] -= 10;
        break;
     /* ROLL */
    case 'a':
        channels[ROLL_INDEX] -= 10;
        break;
    case 'd':
        channels[ROLL_INDEX] += 10;
        break;
    /* ARM */
    case 'z':
        channels[ARM_INDEX] = 1600;
        break;
    case 'x':
        channels[ARM_INDEX] = 900;
        break;
    default:
        std::cout << "Invalid key: " << key << std::endl;
        return;
    }
}

void Keyboard::handleInput() {
    mapKeyToChannel(getKeypress());
}