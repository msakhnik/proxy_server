#include "ProxyServer.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <boost/smart_ptr.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

cProxyServer::cProxyServer() : _port(10000),
                               _max_connections(1000000)
{
    p = new struct addrinfo;
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;
}

cProxyServer::~cProxyServer()
{
    delete p;
}

bool cProxyServer::StartServer()
{
    struct addrinfo *res;
    struct addrinfo *p = new struct addrinfo;
    string port_str = boost::lexical_cast <string>(_port);
    if (getaddrinfo(NULL, port_str.c_str(), &_hints, &res) != 0)
    {
        cerr << "getaddrinfo() error" << endl;
        return false;
    }
    _listenfd = socket(p->ai_family, p->ai_socktype, 0);
    for (p = res; p != NULL; p = p->ai_next)
    {
        _listenfd = socket(p->ai_family, p->ai_socktype, 0);
        if (_listenfd == -1)
            continue;
        if (bind(_listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
    }
    if (p == NULL)
    {
        cerr << "socket() or bind()" << endl;
        freeaddrinfo(res);
        return false;
    }
    freeaddrinfo(res);
    if (listen(_listenfd, _max_connections) != 0)
    {
        cerr << "listen() error" << endl;
        return false;
    }
    return true;
}
