#pragma once
#include <unistd.h>
#include "timer.hpp"
#include "configuration.hpp"
#include "Network.hpp"
#include "authorization.hpp"

class Connection
{
    bool Status;
    Timer Con_Check; // Updates every message. Used for closing connection if no message arrives in 2 seconds.
  public:
    Connection() {Status=true;};
    void StartIO(Server &tcpserver, HOMEKIT &device);
    void IOHandler(Server tcpserver, HOMEKIT device);
    const bool GetStatus();
};