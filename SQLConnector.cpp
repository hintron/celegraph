#include "SQLConnector.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "Utils.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>

// TODO: Make all strings references to reduce copying

// For how to use sqlitecpp, see https://github.com/SRombauts/SQLiteCpp/blob/master/examples/example1/main.cpp

// Note: In c++, you can't just instantiate a class inside the constructor - it needs to be in the initialization list
// This tries to open the database file when instantiated, and set it to write
SQLConnector::SQLConnector() : db("db/celegraph.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE){
    std::cout << "Celegraph database \"" << db.getFilename() << "\" opened successfully." << std::endl;
}

// TODO: There is no need to free the db, right? Should be automatic?
SQLConnector::~SQLConnector() {}


bool SQLConnector::ExecuteSqlFile(std::string filename) {
    // Extract the contents of the sql file into a stringstream
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    // Execute the sql file
    try {
        db.exec(buffer.str());
        return true;
    }
    catch (std::exception& e) {
        std::cout << "Exception: could not execute sql file:" << std::endl;
        std::cout << e.what() << std::endl;
        return false;
    }
}



// Executes a static SQL statement, and prints out the result
// Returns true if successful, false otherwise
bool SQLConnector::Execute(std::string cmd, bool quiet) {
    try {
        SQLite::Statement query(db, cmd);
        if(!quiet){
            std::cout << "Attempting to execute the following query:" << std::endl;
            std::cout << query.getQuery() << std::endl;
        }
        int i = 0;
        bool printed_columns = false;
        while (query.executeStep()) {
            if(quiet){
                i++;
                continue;
            }
            int col_count = query.getColumnCount();
            if(!printed_columns){
                printed_columns = true;
                std::cout << "Columns: ";
                for (int k = 0; k < col_count; ++k) {
                    // To enable getColumnOriginName(), make sure SQLITE_ENABLE_COLUMN_METADATA is defined during compilation
                    std::cout << std::setw(10) << query.getColumnOriginName(k) << " | ";
                }
                std::cout << std::endl;
            }
            std::cout << "Row " << std::setw(3) << i << ": ";
            for (int j = 0; j < col_count; ++j){
                std::cout << std::setw(10) << query.getColumn(j) << " | ";
            }
            std::cout << std::endl;
            i++;
        }
        return true;
    }
    catch (std::exception& e) {
        std::cout << "Exception: could not execute sql statement:" << std::endl;
        std::cout << e.what() << std::endl;
        return false;
    }
}


//////////////////////////////
//      CREATE
//////////////////////////////


// Insert a new account record.
int SQLConnector::InsertAccount(std::string accountName, std::string email, std::string key, std::string salt) {
    // Sanitize key, salt, and account name
    if (!utils::SanitizeAccountName(accountName)) {
        std::cout << "Account Name is invalid! Cannot insert account record" << std::endl;
        return false;
    }
    // TODO: Sanitize email!
    // if (!utils::SanitizeEmail(email)) {
    //     std::cout << "Email is invalid! Cannot insert account record" << std::endl;
    //     return false;
    // }
    if (!utils::SanitizeHexString(key)) {
        std::cout << "Key is invalid! Cannot insert account record" << std::endl;
        return false;
    }
    if (!utils::SanitizeHexString(salt)) {
        std::cout << "Salt is invalid! Cannot insert account record" << std::endl;
        return false;
    }
    std::string query_string("insert into accounts (accountname, valid, email, playing, key, salt) values (?,?,?,?,?,?)");
    try {
        SQLite::Statement query(db, query_string);
        query.bind(1, accountName);
        query.bind(2, true);
        query.bind(3, email);
        query.bind(4, true);
        query.bind(5, key);
        query.bind(6, salt);
        int rows_modified = query.exec();
        if(rows_modified < 1) {
            std::cout << "Could not insert account record! " << rows_modified << " rows were modified" << std::endl;
            return 0;
        }
        else {
            return db.getLastInsertRowid();
        }
    }
    catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        std::cout << query_string << std::endl;
        return 0;
    }
}



// Inserts a new user into the database.
int SQLConnector::InsertUser(User u, int account_id) {
    std::cout << "Creating user " << u.GetFirstname() << " " << u.GetLastname() << std::endl;

    // TODO: Deal with these fields
    // sockaddr_in client,
    // std::string account,
    // int id,
    // int session,

    // TODO: Update active user list


    // Now insert the user using the newly-created user
    std::string query_string(R"(
        INSERT INTO user (
            account_id,
            -- Identification:
            firstname,
            lastname
        ) VALUES (
            :account_id,
            -- Identification
            :firstname,
            :lastname
        )
    )");

    try {
        SQLite::Statement query(db, query_string);
        query.bind(":account_id", account_id);
        // Identification
        query.bind(":firstname", u.GetFirstname());
        query.bind(":lastname", u.GetLastname());

        int rows_modified = query.exec();

        if(rows_modified < 1){
            std::cout << "Could not insert user record! " << rows_modified << " rows were modified" << std::endl;
            return 0;
        }
        else {
            // Get the id of the newly inserted record
            return db.getLastInsertRowid();
        }
    }
    catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        std::cout << query_string << std::endl;
        return 0;
    }
}



//////////////////////////////
//      READ
//////////////////////////////

std::vector<std::string> SQLConnector::GetAccounts() {
    std::vector<std::string> accounts;

    std::string cmd("SELECT accountname FROM accounts ORDER BY accountname");
    SQLite::Statement query(db, cmd);
    while (query.executeStep()) {
        accounts.push_back(query.getColumn(0));
    }
    return accounts;
}


std::vector<std::string> SQLConnector::GetUserList(std::string accountname) {
    std::vector<std::string> users;

    std::string cmd("SELECT firstname, lastname, accountname FROM view_users WHERE accountname = ? ORDER BY lastname");
    SQLite::Statement query(db, cmd);
    query.bind(1, accountname);

    while (query.executeStep()) {
        std::string name = query.getColumn(0);
        std::string lastname = query.getColumn(1);
        name.append(" ").append(lastname);
        users.push_back(name);
    }
    return users;
}

// Return the salt for the passed account
bool SQLConnector::GetAccountSalt(char *accountName, char *saltStringHex) {
    // Sanitize the account name before preceeding
    if (!utils::SanitizeAccountName(accountName)) {
        return false;
    }
    char *errorMessage = NULL;
    std::stringstream query;
    query << "select salt from accounts where accountname = \"" << accountName << "\"";
    // TODO: Get the salt

    if (utils::SanitizeHexString(saltStringHex)) {
        return true;
    }
    else {
        return false;
    }

}

// Returns the key to the passed account
int SQLConnector::GetAccountKey(char *accountName, char *keyStringHex) {
    // // Sanitize the account name before preceeding
    // if (!utils::SanitizeAccountName(accountName)) {
    //     return 0;
    // }
    // char *errorMessage = NULL;
    // std::stringstream query;
    // query << "select key from accounts where accountname = \"" << accountName << "\"";
    // //std::cout << query.str() << std::endl;
    // // The fourth param is passed to the callback function as a void pointer to the first param
    // sqls = sqlite3_exec(database, query.str().c_str(), &ReturnStringCallback, keyStringHex, &errorMessage);
    // if (sqls != SQLITE_OK) {
    //     std::cout << "Could not Execute SQL query! Return Code:" << sqls << std::endl;
    // }
    // if (errorMessage) {
    //     // Print out the error message if any
    //     std::cout << "SQL ERROR MESSAGE: " << errorMessage << std::endl;
    //     // Free the error message, since it was alloced in exec()
    //     sqlite3_free(errorMessage);
    // }
    return 1;
}



//////////////////////////////
//      UPDATE
//////////////////////////////





//////////////////////////////
//      DELETE
//////////////////////////////




//////////////////////////////
//      PRINT
//////////////////////////////


// Lists all the accounts
void SQLConnector::ListAccounts() {
    std::vector<std::string> accounts = GetAccounts();
    std::cout << "ACCOUNTS: " << std::endl;
    for (int i = 0; i < accounts.size(); ++i) {
        std::cout << accounts.at(i) << std::endl;
    }
}
