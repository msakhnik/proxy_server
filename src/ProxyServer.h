#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#include <sys/epoll.h>

class cProxyServer
{
public:
    cProxyServer();
    ~cProxyServer();
    void SetPort(int num) { _port = num; };
    int GetPort() { return _port; };
    int CreateConnection();
    bool StartServer();
private:
    int _port;
    struct sockaddr_in _clientaddr;
    struct addrinfo *p;
    socklen_t _addrlen;
    struct addrinfo _hints;
    int _listenfd;
    int _max_connections;
};