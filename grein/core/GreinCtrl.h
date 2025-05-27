#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "CRSF.h"
#include "Serial.h"
#include "UDPServer.h"

#include <iostream>
#include <cstring>
#include <arpa/inet.h>

class GreinCtrl {
public:
    GreinCtrl() = delete;
    GreinCtrl(int serverPort, const char* uartPort);
    ~GreinCtrl();

    void run();
    void sendPeriodicMessage();

private:
    UDPServer server;
    CRSF crsf{};
    Serial uart;
};

#endif // UDP_SERVER_H
