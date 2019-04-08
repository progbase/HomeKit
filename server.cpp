#include <iostream>
#include <csignal>
#include "NetworkIO.hpp"
#include "arduino-serial-lib.h"
#include "configuration.hpp"
#include <thread>

#define SERIAL_PORT "/dev/ttyACM0"
#define SOC_PORT 8888
#define MAX_CONNECTIONS 1

int main()
{
    int SerialStream = serialport_init(SERIAL_PORT, 19200);
    socklen_t len = sizeof(int);
    HOMEKIT Device;
    //Device.ConfigurationFile_Read();
    Device.config(true);
    Device.set_name("Homekit");
    Device.set_id("1918");
    Device.set_secure(false);
    Server TCPserver(SOC_PORT);
    TCPserver.soc_bind();
    TCPserver.listen(MAX_CONNECTIONS);
    std::cout << "Waiting for connection" << std::endl;
    TCPserver.accept_connection(len);
    Connection connection;
    // std::thread ConUp(connection.StartIO& , TCPserver);

    connection.StartIO(TCPserver, Device);
    return 0;
}
