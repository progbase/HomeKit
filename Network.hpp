#pragma once
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <iostream>

#define CONCLOSE_MESSAGE "CLOSED\n"

class Server
{
    int listenfd;
    int connfd;
    fd_set rset;
    struct sockaddr_in cliaddr, servaddr;
  public:
    Server(uint16_t port)
    {
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
        connfd = 0;
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(port);
    }

    int GetConID() { return connfd; };
    void Close()
    {
        write(connfd, CONCLOSE_MESSAGE, 9);
        close(connfd);
        std::cout << "Connection closed" << std::endl;
    }

    void soc_bind()
    {
        bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }

    void listen(const int Connection_Number)
    {
        ::listen(listenfd, Connection_Number);
        FD_ZERO(&rset);
        FD_SET(listenfd, &rset);
    }

    void accept_connection(socklen_t length)
    {
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &length);
    }

    char * read_message()
    {
        char * buffer = new char[256];
        read(connfd, buffer, sizeof(buffer));
        return &buffer[0];
    }
};
