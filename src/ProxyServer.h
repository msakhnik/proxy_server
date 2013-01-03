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
#include <vector>

class cProxyServer
{
public:
    cProxyServer();
    ~cProxyServer();
    void SetPort(int num) { _port = num; };
    int GetPort() { return _port; };
    bool CreateConnection();
    bool StartServer();    
private:
    int _port;
    struct sockaddr_in _clientaddr;
    struct addrinfo *p;
    socklen_t _addrlen;
    struct addrinfo _hints;
    int _listenfd;
    int _max_connections;
    std::vector<int> _clients;
    static const int _conmax = 1000;
    static const int _message_lenght = 999999;
    int _efd;
    struct epoll_event _event;
    struct epoll_event *_events;
    bool _InitEpol();
    void  _Respond(int n);
};