#include "GreinCtrl.h"

constexpr char UART_PORT[] = "/dev/ttyAMA0";

/* TODO: Make try : catch + exceptions inside the code, instead of returns of ints and bools */
int main() {
    GreinCtrl app(8070, UART_PORT);

    app.run();

    return 0;
}