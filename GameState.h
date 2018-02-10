// Filename:    GameState.h
// Author:      Joseph DeVictoria
// Date:        Mar_2_2016
// Purpose:     Header for Game State class.
//              This class will be responsible for maintaining the Players,
//              NPCs necessary to preserve the game state.  It will also
//              perform the bulk of database interactions.

#ifndef OLDENTIDE_GAMESTATE_H
#define OLDENTIDE_GAMESTATE_H

#include "Npc.h"
#include "Player.h"
#include "SQLConnector.h"
#include <set>
#include <string>
#include <vector>
#include <map>

class Server;

class GameState{
    public:
        GameState(Server * server, SQLConnector * sql);
        ~GameState();
        bool VerifySession(int sessionId);
        bool VerifyActiveSession(int sessionId);
        void DisconnectSession(int sessionId);
        void PlayerCommand(std::string command, int sessionId);
        void SelectPlayer(int sessionId);
        int GenerateSession(int sessionId);
        void SetSessionAccountName(char * accountName, int sessionId);
        std::string GetSessionAccountName(int sessionId);
        std::vector<Player> getPlayers();
        std::vector<Npc> getNPCs();

    private:
        SQLConnector * sql;
        Server * server;
        std::vector<Player> players;
        std::vector<Npc> npcs;
        std::set<int> sessions;
        std::set<int> activeSessions;
        std::map<int, std::string> sessionAccounts;
        int curSession;
        //Player ReadPlayer(std::string name);
        void StorePlayer(std::string name);
};

#endif // OLDENTIDE_GAMESTATE_H
