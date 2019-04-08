#include <iostream>
#include "arduino.hpp"


void SerialPortWrite(const char * write_data, int ser_port)
{
    try
    {
    serialport_write(ser_port, write_data);
    }
    catch(const std::exception& e)
    {

    }
}
