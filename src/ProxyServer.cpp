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
    cerr << "in construct" << endl;
    _clients.resize(_conmax, -1);
}

cProxyServer::~cProxyServer()
{
    delete p;
}

bool cProxyServer::StartServer()
{
    struct addrinfo *res;
    struct addrinfo *p = new struct addrinfo;
    string port_str = boost::lexical_cast <string > (_port);
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

bool cProxyServer::CreateConnection()
{
    int slot = 0;
    while (1)
    {
        _addrlen = sizeof (_clientaddr);
        _clients.at(slot) = accept(_listenfd, (struct sockaddr *)
                                   &_clientaddr, &_addrlen);
        if (_clients.at(slot) < 0)
        {
            cerr << "accept() error" << endl;
            return false;
        }
        else
        {
            cerr << "wait" << endl;
            if (!_InitEpol())
                return false;
            
            cerr << _efd << "\t" << endl;
            int n = epoll_wait(_efd, _events, _conmax, -1);
            cerr << n << endl;
            for (int i = 0; i < n; i++)
            {
                if ((_events[i].events & EPOLLERR) ||
                        (_events[i].events & EPOLLHUP) ||
                        (!(_events[i].events & EPOLLIN)))
                {
                    cerr << "Error in epol" << endl;
                    close(_events[i].data.fd);
                    continue;
                }
                else
                {
                    _Respond(_events[i].data.fd);
                }
            }
        }
        while (_clients.at(slot) != -1) slot = (slot + 1) % _conmax;
    }
    return true;
}

bool cProxyServer::_InitEpol()
{
    int s = 0;
    _efd = epoll_create1(0);
    if (_efd == -1)
    {
        perror("epoll_create");
        abort();
    }

    printf("sock=%d\n", _listenfd);
    _event.data.fd = _listenfd;
    _event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(_efd, EPOLL_CTL_ADD, _listenfd, &_event);
    if (s == -1)
    {
        cerr << "epoll_ctl() ERROR" << endl;
        return false;
    }

    /* Buffer where events are returned */
    _events = (struct epoll_event*) calloc(_conmax, sizeof _event);
    return true;
}

//client connection

void cProxyServer::_Respond(int n)
{
    char mesg[_message_lenght];
    int rcvd;

    memset((void*) mesg, (int) '\0', _message_lenght);
    cerr << n << endl;

    rcvd = recv(_clients.at(n), mesg, _message_lenght, 0);
    cerr << rcvd << endl;

    if (rcvd < 0) // receive error
        cerr << "recv() error" << endl;
    else if (rcvd == 0) // receive socket closed
        cerr << "Client disconnected upexpectedly." << endl;
    else // message received
    {
        cout << mesg << endl;
//        GetHttpHeader(mesg, n);
    }

    //Closing SOCKET
    if (shutdown(_clients.at(n), SHUT_RDWR) == 0)
    {
        printf("Close client request №%d\n", n);
        close(_clients.at(n));
        _clients.at(n) = -1;
    }
}
