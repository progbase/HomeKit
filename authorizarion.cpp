#include "authorization.hpp"

bool Secured_Authorization(HOMEKIT device, std::string password)
{
    if(device.get_pin() == password) return true;
    else return false;
}
