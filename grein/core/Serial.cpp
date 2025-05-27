#include "Serial.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <asm/termbits.h>
#include <sys/ioctl.h>
#include <cstring>
#include <errno.h>

Serial::Serial(const char* port) {
    configurePort(port);
}

Serial::~Serial() {
    closePort();
}

bool Serial::configurePort(const char *port) {
    fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd == -1) {
        std::cerr << "Failed to open port: " << strerror(errno) << std::endl;
        return false;
    }

    struct termios2 tty;
    memset(&tty, 0, sizeof(tty));
 
    if (ioctl(fd, TCGETS2, &tty) != 0) {
        std::cerr << "Failed to get port attributes: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit data
    tty.c_cflag &= ~PARENB;                      // No parity
    tty.c_cflag &= ~CSTOPB;                      // 1 stop bit
    tty.c_cflag |= CREAD | CLOCAL;               // Enable receiver and local mode

    /* Disable hardware flow control (RTS/CTS) */
    tty.c_cflag &= ~CRTSCTS;

    /* Configure input flags */
    tty.c_iflag = IGNPAR;

    /* Configure output flags */
    tty.c_oflag = 0;
    
    /* Configure local flags */
    tty.c_lflag = 0;

    /* Setting timeouts */
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 10;

    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= BOTHER;
    /* Setting baudrate (for CRSF - 400k) */
    tty.c_ispeed = CRSF_BAUDRATE;
    tty.c_ospeed = CRSF_BAUDRATE;

    /* Clean buffers and apply configuration */
    if (ioctl(fd, TCFLSH, TCIOFLUSH) != 0) {
        std::cerr << "Failed to flush port buffers: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }
 
    if (ioctl(fd, TCSETS2, &tty) != 0) {
        std::cerr << "Configuration error: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    if (ioctl(fd, TCGETS2, &tty) != 0) {
        std::cerr << "Failed to get port attributes: " << strerror(errno) << std::endl;
        close(fd);
        return false;
    }

    std::cout << "UART had been initialized on:" << port << std::endl;
    std::cout << "Baud rate: " << tty.c_ospeed << std::endl;
    std::cout << "Data bits: " << ((tty.c_cflag & CSIZE) == CS8 ? 8 : 7) << std::endl;
    std::cout << "Stop bits: " << (tty.c_cflag & CSTOPB ? 2 : 1) << std::endl;
    std::cout << "Parity: " << ((tty.c_cflag & PARENB) ? "Even" : "None") << std::endl;
    std::cout << "Flow control: " << ((tty.c_cflag & CRTSCTS) ? "Enabled" : "Disabled") << std::endl;

    return true;
}

void Serial::closePort() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

int Serial::readData(unsigned char *buffer, int length) {
    if (fd == -1) {
        std::cerr << "Port is unintialized!" << std::endl;
        return -1;
    }

    int bytes_read = read(fd, buffer, length);
    if (bytes_read < 0) {
        std::cerr << "Failed to read data: " << strerror(errno) << std::endl;
        return -1;
    }
    return bytes_read;
}

int Serial::writeData(const unsigned char *data, int length) {
    if (fd == -1) {
        std::cerr << "Port is uninitialized!" << std::endl;
        return -1;
    }

    int bytes_written = write(fd, data, length);
    
    if (bytes_written < 0) {
        std::cerr << "Failed to write data: " << strerror(errno) << std::endl;
        return -1;
    }

    return bytes_written;
}
