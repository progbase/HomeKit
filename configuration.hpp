#pragma once

#include <iostream>
#include <string>

class HOMEKIT{
    bool configured;
    std::string Name;
    std::string id;
    bool secured;
    std::string pin;
    std::string client_id;

    public:
    HOMEKIT()
        {
            Name = "UNNAMED";
            id = "NO ID";
            secured = false;
            pin="NO PIN";
            configured = false;
            client_id = "NO_CLIENT_ID";
        }
    ~HOMEKIT()
    {
       
    }
    std::string get_name() {return Name;}
    std::string get_id() {return id;}
    std::string get_pin() {return pin;}
    bool is_secured() {return secured;}
    std::string get_client_id() {return client_id;}
    bool is_configured() {return configured;}

    void set_name(std::string name) { Name = name;}
    void set_id(std::string ID) { id = ID;}
    void set_pin(std::string PIN) { pin = PIN;}
    void set_secure(bool SECURED) { secured = SECURED;}
    void config(bool CONFIG) {configured = CONFIG;}
    void set_client_id(std::string CLIENT_ID) {client_id = CLIENT_ID;}
    void ConfigurationFile_Read();
    void ConfigurationFile_Write();
}; 