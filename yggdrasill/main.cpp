#include "YggdrasillCtrl.h"

#define OPENHD_IP "192.168.3.1"
#define LOCAL_IP "127.0.0.1"

/* TODO: Make try : catch + exceptions inside the code, instead of returns of ints and bools */
int main() {
    YggdrasillCtrl app(OPENHD_IP, 8070, InputDevice::RealFlightRC);

    app.run();

    return 0;
}