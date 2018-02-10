// Filename:    SQLConnector.h
// Author:      Joseph DeVictoria
// Date:        Feb_29_2016
// Purpose:     Header for SQL Connector class.

#ifndef OLDENTIDE_SQLCONNECTOR_H
#define OLDENTIDE_SQLCONNECTOR_H

#include "SQLiteCpp/SQLiteCpp.h"
#include "Character.h"
#include "Player.h"
#include "Item.h"
#include "Npc.h"
#include <string>
#include <vector>
// #include <set>

class SQLConnector{
    public:
        SQLConnector();
        ~SQLConnector();
        bool Execute(std::string cmd, bool quiet = false);
        bool ExecuteSqlFile(std::string filename);

        //////////
        // Create
        //////////
        int InsertAccount(std::string, std::string, std::string, std::string);
        int InsertPlayer(Player newPlayer, int account_id);
        int InsertNpc(Npc newNpc);
        int InsertItem(Item newItem);

        //////////
        // Read
        //////////
        std::vector<std::string> GetAccounts();
        std::vector<std::string> GetPlayerList(std::string account);
        std::vector<Npc> GetNpcs();
        bool GetAccountSalt(char *, char *);
        int GetAccountKey(char *, char *);

        //////////
        // Update
        //////////

        //////////
        // Delete
        //////////

        //////////
        // Print
        //////////
        void ListAccounts();

    private:
        SQLite::Database db;
};

#endif // OLDENTIDE_SQLCONNECTOR_H
