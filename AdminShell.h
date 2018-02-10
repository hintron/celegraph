// Filename:    AdminShell.h
// Author:      Joseph DeVictoria
// Date:        Mar_6_2016
// Purpose:     Header for the Administrator shell class used to implement
//              server commands.

#ifndef OLDENTIDE_ADMINSHELL_H
#define OLDENTIDE_ADMINSHELL_H

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

#endif // OLDENTIDE_ADMINSHELL_H
