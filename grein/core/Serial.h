#pragma once

#define CRSF_BAUDRATE 400000
#define SBUS_BAUDRATE 100000

class Serial {
public:
    Serial() = delete;
    Serial(const char* port);

    ~Serial();

    int readData(unsigned char *buffer, int length);
    int writeData(const unsigned char *data, int length);
private:
    int fd;
    bool configurePort(const char *port);
    void closePort();
};