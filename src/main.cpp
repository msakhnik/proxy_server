#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include <cstring>
#include <vector>

#include "ProxyServer.h"

using namespace std;

static char const* _Basename(char const* fname)
{
    char const* res = strrchr(fname, '/');
    return res ? res + 1 : fname;
}

static void _ReadHelp(const char *progname)
{
    cout << "Smart_pc\n\n"
            "Synopsis:\n"
            " " << progname << " [options]\n\n"
            "Options:\n"
            " -p,--port\t\tSet port\n"
            " -h,--help\t\tThis is help message\n\n"
            "Example:\n"
            << endl;
}

int main(int argc, char** argv)
{

    char const* progname = _Basename(argv[0]);
    cProxyServer server;

    while (true)
    {
        static struct option long_options[] = {
            { "port", required_argument, 0, 'p'},
            { "help", no_argument, 0, 'h'},
            { 0, 0, 0, 0}
        };

        int option_index = 0;

        int c = getopt_long(argc, argv, "p:h",
                            long_options, &option_index);

        if (c == -1)
        {
            break;
        }

        switch (c)
        {
        case 'p':
            cerr << "opt - p" << endl;
            server.SetPort(atoi(optarg));
            break;
        case 'h':
            _ReadHelp(progname);
            return 0;

        default:
            cerr << "Unknown option '" << c << "'" << endl;
            return 1;
        }
    }
    if  (server.StartServer())
    {
        cout << "Connection was succesfully created" << endl;
        cout << "Listening" << endl << endl;
        
        if (server.CreateConnection())
            cerr << "Close connection" << endl;
    }
        
    return 0;
}