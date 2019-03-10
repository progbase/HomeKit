
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>

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

#include "arduino-serial-lib.h"

#define PORT 8888
#define MAXLINE 1024

int ForTest = 0;
const char KEYWORD[9] = "HOMEKIT\n";
int global_connection;
int global_serial;
clock_t start = 0, last = 0;
bool CONNECTION_STATUS = true;
bool TIME_ELAPSED = false;
bool TECHNICAL_VAR = false;

int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

void *writetoserial(void *str)
{
    serialport_write(ForTest, str);
}

bool Authorization(char *password)
{
    if (!strcmp(password, KEYWORD))
        return true;
    else
        return false;
}

void Authorized(int *connection)
{
    char response[8];
    strcpy(response, KEYWORD);
    response[7] = '\n';
    puts("Connection estabilished with client");
    write(*connection, (const char *)response, sizeof(response));
}

void func_exit()
{
    write(global_connection, "CLOSED\n", 9);
    close(global_connection);
    //   serialport_close(global_serial);
    printf("\nTERMINATED. CONNECTION CLOSED\n");
    exit(0);
}

void *Socket_Check(void *len)
{
    int retval = 0;
    int error = 0;
    while (CONNECTION_STATUS)
    {
        retval = getsockopt(global_connection, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
        printf("%d", CONNECTION_STATUS);
        if (retval != 0)
        {
            CONNECTION_STATUS = false;
            break;
        }
        printf("\nSocket value %d\n", retval);
        sleep(2);
    }
}

void *time_check()
{
    while (CONNECTION_STATUS)
    {
        if (TECHNICAL_VAR)
        {

            last = clock();
            printf("LAST: %lf", (double)last);
            printf("START: %lf", (double)start);
            sleep(1);
            if ((last - start) > 2 * CLOCKS_PER_SEC)
            {
                TIME_ELAPSED = true;
                break;
            }
        }
    }
}

void *IOProcessor()
{
    char command[9];

    start = clock();
    while (CONNECTION_STATUS)
    {
        bzero(command, 9);

        if (CONNECTION_STATUS)
        {
            int len = read(global_connection, command, 9);
            TECHNICAL_VAR = true;
            if (CONNECTION_STATUS)
            {

                if (command[0] != '\0')
                {
                    printf("Connection: %d", global_connection);
                    printf("%lu\n", sizeof(command));
                    printf("%d\n", len);

                    if (!strcmp(command, "PING\n"))
                    {
                        printf("Pinged\n");
                        start = clock();
                    }
                    else
                    {
                    printf("LAST: %lf", (double)last);
                    printf("START: %lf", (double)start);
                    if (!strcmp(command, "CLOSED\n"))
                    {
                        write(global_connection, "CLOSED\n", 9);
                        close(global_connection);
                        // serialport_close(fd);
                        CONNECTION_STATUS = false;
                        // pthread_join(wrt, NULL);
                        break;
                    }
                    printf("Message from client:%s", command);
                    command[8] = '\n';
                    // retrn =   fprintf(arduino,"%s", buffer);
                    printf("Size of command  %lu\n", sizeof(command));
                    // ForTest = fd;
                    //              pthread_create(&wrt, NULL, writetoserial, (void *)&command[0] );
                    serialport_write(global_serial, command);
                    //                  pthread_join(wrt, NULL);
                    start = clock();
                    printf("SENT TO PORT\n ");
                    }
                }
            }
        }
    }
}

int main()
{
    signal(SIGTSTP, func_exit);
    signal(SIGINT, func_exit);
    //atexit(func_exit);
    pthread_t ZA_WARUDO;
    pthread_t wrt;
    pthread_t iread;
    int fd = serialport_init("/dev/ttyACM0", 19200);
    int listenfd, connfd, udpfd, nready, maxfdp1;
    char buffer[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;

    char message[5] = "test";
    message[4] = '\n';
    void sig_chld(int);

    /* create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // binding server addr structure to listenfd
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 10);
    // clear the descriptor set
    FD_ZERO(&rset);
    char msg[9] = "m100100\n";
    FD_SET(listenfd, &rset);
    fflush(stdout);
    int error = 0;
    socklen_t length = sizeof(error);
    int retval = 0;
    while (1)
    {
        FD_ZERO(&rset);
        FD_SET(listenfd, &rset);
        printf("Waiting for connection\n");
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        bzero(buffer, sizeof(buffer));
        read(connfd, buffer, sizeof(buffer));
        puts(buffer);
        bool Authorization_Check = false;
        global_connection = connfd;
        global_serial = fd;
        Authorization_Check = Authorization(&buffer[0]);
        CONNECTION_STATUS = true;
        TIME_ELAPSED = false;
        pthread_create(&wrt, NULL, Socket_Check, (void *)&len);

        if (Authorization_Check)
        {
            // Authorized(&connfd);
            char response[8];
            strcpy(response, KEYWORD);
            response[7] = '\n';
            puts("Connection estabilished with client");
            write(connfd, (const char *)response, sizeof(response));
            char command[9];
            
            pthread_create(&iread, NULL, IOProcessor, NULL);
            pthread_create(&ZA_WARUDO, NULL, time_check, NULL);
            while (1)
            {
                /*  if(retval!=0)
                {
                    close(connfd);
                    Authorization_Check = false;
                    break;
                }
                */

                if (CONNECTION_STATUS)
                {
                    if (TIME_ELAPSED)
                    {
                        CONNECTION_STATUS = false;
                        puts("IT JUST WORKS");
                        pthread_cancel(iread);
                        pthread_cancel(ZA_WARUDO);
                        printf("\nRead tread closed\n");
                        puts("Disconnected\n");
                        close(connfd);
                        // serialport_close(fd);
                        Authorization_Check = false;
                        break;
                    }
                }
                else
                {
                    pthread_join(iread, NULL);
                    pthread_join(ZA_WARUDO, NULL);
                    printf("\nRead tread closed\n");
                    puts("Disconnected\n");
                    close(connfd);
                    // serialport_close(fd);
                    Authorization_Check = false;
                    break;
                }
            }
            printf("\nSocket check tread closed\n");
            pthread_cancel(wrt);
            
        }
        else
        {
            char response[6] = "ERROR";
            response[5] = '\n';
            puts("Connection refused\n");
            for (int i = 0; i < 8; i++)
            {
                printf("%c", buffer[i]);
            }
            printf("%s", buffer);
            write(connfd, (const char *)response, sizeof(response));
            close(connfd);
        }
    }
    //  close(connfd);
    serialport_close(fd);
    //  fclose(arduino);
}
