// SQL Connector class to handle SQLite3 queries.

#ifndef CELEGRAPH_SQLCONNECTOR_H
#define CELEGRAPH_SQLCONNECTOR_H

#include "SQLiteCpp/SQLiteCpp.h"
#include "User.h"
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
        int InsertUser(User newUser, int account_id);

        //////////
        // Read
        //////////
        std::vector<std::string> GetAccounts();
        std::vector<std::string> GetUserList(std::string account);
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

#endif // CELEGRAPH_SQLCONNECTOR_H
