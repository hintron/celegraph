// Provides an interface for the administrator to interact with
// the server.  This class will handle all server admin commands.

#ifndef CELEGRAPH_ADMINSHELL_H
#define CELEGRAPH_ADMINSHELL_H

#include "GameState.h"
#include "SQLConnector.h"
#include <limits.h>
#include <unistd.h>
#include <vector>

// forward declaration so AdminShell and Server can see each other
// See http://stackoverflow.com/questions/4964482/how-to-create-two-classes-in-c-which-use-each-other-as-data
class Server;

class AdminShell {
    public:
        AdminShell(Server * server, SQLConnector * sql, GameState * gameState);
        ~AdminShell();
        void operator()();
        void Run();
        void PrintUsage();
        void PrintLogo();
    private:
        Server * server;
        SQLConnector * sql;
        GameState * gameState;
        char serverHostname[HOST_NAME_MAX];
};

#endif // CELEGRAPH_ADMINSHELL_H
