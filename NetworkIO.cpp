
#include "NetworkIO.hpp"
#include <string>
#include <cstring>
#include <thread>

#define CONCLOSE_MESSAGE "CLOSED\n"
#define PING "PING\n"
#define SERIAL_PORT "/dev/ttyACM0"

void Connection::StartIO(Server &tcpserver, HOMEKIT &device)
{
    //std::thread IO(&Connection::IOHandler, this, tcpserver);
    std::thread IO([this, tcpserver, device] {
        this->IOHandler(tcpserver, device);
    });
    while (Status)
    {
        //std::cout << "ELAPSED SECONDS:" << Con_Check.elapsedSeconds() << std::endl;
        if (Con_Check.elapsedSeconds() > 5)
        {
            Status = false;
           tcpserver.Close();
           std ::cout << "NO RESPONSE" << std ::endl;
            break;
        }
    }
    IO.join();
}

bool Auth(Server tcpserver, HOMEKIT device)
{
    //char command[100];
    //read(tcpserver.GetConID(), command, 100);
    //command[8]='\0';
    //if (!strcmp(command, "HOMEKIT\n"))
    //{
        write(tcpserver.GetConID(), "HOMEKIT\n", 8);
        std::string Auth_key;
        std::cout << "Authorized" << std::endl;
        if (device.is_configured())
        {
            Auth_key = "A";
            if (device.is_secured())
            {
                Auth_key+="S";
            }
            Auth_key+="N"+device.get_name()+"\t"+"I"+device.get_client_id()+"\t\n";
            std::cout << Auth_key;
        }
        else
        {
            // TODO: Implement
        }
        
        write(tcpserver.GetConID(), Auth_key.c_str(), sizeof(Auth_key)+1);
        return true;
    
    //else
    //{
    //    return false;
    //}
}

void Connection::IOHandler(Server tcpserver, HOMEKIT device)
{
    char command[100];
    Con_Check.start();
    if(Auth(tcpserver, device))
    {
        // TODO
        if(device.is_secured())
        {

        }
    }
    else
    {
        Status = false;
        return;
    }

    Con_Check.start();
    while (Status)
    {
        int length = read(tcpserver.GetConID(), command, 100);
        if (!Status)
            break;
        if (length > 0)
        {
            Con_Check.start();
            if (!strcmp(command, PING))
            {
                std::cout << "Pinged" << std::endl;
                bzero(&command, 100);
                continue;
            }
            if (!strcmp(command, CONCLOSE_MESSAGE))
            {
                tcpserver.Close();
                Status = false;
                break;
            }
            if (command[0] == 'D')
            {
                std::string settings = command;
                std::cout << "Settings:" << settings << std::endl;
                settings.erase(0, 1);
                std :: string name;
                std :: string password;
                name = settings.substr(0, settings.find("\t"));
                std::cout << "Name:" << name << std::endl;
                device.set_name(name);
                int last_delimiter_pos = settings.find("\t");
                settings.erase(0, last_delimiter_pos+1);
                if(settings.length()>2)
                {
                    password = settings.substr(last_delimiter_pos, settings.find("\t"));
                    std::cout << "Password:" << settings << std::endl;
                    device.set_pin(password);
                }
                write(tcpserver.GetConID(), "OK\n", 9);
                std::cout << "CHANGED NAME:" << device.get_name() << std::endl;
                continue;
            }
            std::cout << "Message from client:" << command << std::endl;
            if (strlen(command) > 7)
            {
                //serialport_write(global_serial, command);
                //SerialPortWrite(command, "");
            }
            bzero(&command, 100);
        }
    }
}

const bool Connection::GetStatus()
{
    return Status;
}
