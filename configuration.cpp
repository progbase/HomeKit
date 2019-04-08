#include <iostream>
#include <string>
#include "configuration.hpp"

void HOMEKIT::ConfigurationFile_Write()
{
    FILE * fp = fopen("configuration.txt", "w");


    if(this->is_configured())
        fprintf(fp, "configured=YES\n");
    else
        fprintf(fp, "configured=NO\n");

    if(this->is_secured())
        fprintf(fp, "pin=%s\n", (this->get_pin()).c_str());

    fprintf(fp, "name=%s\n", (this->get_name()).c_str());
    fprintf(fp, "id=%s\n", (this->get_id()).c_str());
    fprintf(fp, "client_id=%s", (this->get_client_id()).c_str());

    fclose(fp);
}

std::string read_untill(char delim, FILE * fp, char &symbol)
{
    std::string field;

    while(symbol != delim && symbol != EOF) 
    {
        field+=symbol;
        symbol = fgetc(fp);
    }
    symbol = fgetc(fp);
    return field;
}

void HOMEKIT::ConfigurationFile_Read()
{
    FILE * fp = fopen("configuration.txt", "r");
    if(fp == NULL)
    {
        abort();
    }
    char symbol = fgetc(fp);
    std::string field;


    field = read_untill('=', fp, symbol);

    while(symbol != EOF)
    {
        HOMEKIT device;
        if(field == "configured")
        {
            this->config(true);
            field.clear();

            field = read_untill('\n', fp, symbol);

            if(field == "NO")
            {
                fclose(fp);
                return;
            }
            field.clear();
            field = read_untill('=', fp, symbol);
        }


        if(field == "id")
        {
            field.clear();
            field = read_untill('\n', fp, symbol);
            this->set_id(field);
            field.clear();
            field = read_untill('=', fp, symbol);
        }
    
    
        if(field == "name")
        {
            field.clear();
            field = read_untill('\n', fp, symbol);
            this->set_name(field);
            field.clear();
            field = read_untill('=', fp, symbol);
        }
        
        if(field == "pin")
        {
            field.clear();
            field = read_untill('\n', fp, symbol);
            this->set_pin(field);
            this->set_secure(true);
            field.clear();
            field = read_untill('=', fp, symbol);
        }
        if(field == "client_id")
        {
            field.clear();
            field = read_untill('\n', fp, symbol);
            this->set_client_id(field);
            field.clear();
            field = read_untill('=', fp, symbol);
        }
    }
    fclose(fp);
    return;
}