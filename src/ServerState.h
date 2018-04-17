// Maintains the users and
// perform the bulk of database interactions.

#ifndef CELEGRAPH_SERVERSTATE_H
#define CELEGRAPH_SERVERSTATE_H

#include "SQLConnector.h"
#include <set>
#include <string>
#include <vector>
#include <map>

class Server;

class ServerState{
    public:
        ServerState(Server * server, SQLConnector * sql);
        ~ServerState();
        bool VerifySession(int sessionId);
        bool VerifyActiveSession(int sessionId);
        void DisconnectSession(int sessionId);
        void PlayerCommand(std::string command, int sessionId);
        void SelectPlayer(int sessionId);
        int GenerateSession(int sessionId);
        void SetSessionAccountName(char * accountName, int sessionId);
        std::string GetSessionAccountName(int sessionId);
        std::vector<User> getUsers();

    private:
        SQLConnector * sql;
        Server * server;
        std::vector<User> users;
        std::set<int> sessions;
        std::set<int> activeSessions;
        std::map<int, std::string> sessionAccounts;
        int curSession;
        //Player ReadPlayer(std::string name);
        // void StorePlayer(std::string name);
};

#endif // CELEGRAPH_SERVERSTATE_H
