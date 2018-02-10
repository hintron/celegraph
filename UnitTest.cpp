// A unit testing platform to verify functions.

// Load the testing framework (do it ABOVE the catch include!)
#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include "AdminShell.h"
#include "GameState.h"
#include "User.h"
#include "Packets.h"
#include "Server.h"
#include "SQLConnector.h"
#include "Utils.h"

TEST_CASE( "create server", "[server]" ) {
    Server * server = new Server(7331);
    delete server;
}


// TODO Implement and test these actions:
/////////////////////////////////////////////////////
//       Create and test a 'init db' or blank_db
//       get accounts
//       get users
//       insert account
//       insert user
// TODO: delete account - test cascade deletes
// TODO: delete user
// TODO: update account - test cascade updates
// TODO: update user
/////////////////////////////////////////////////////



///////////////////////////
// SQLConnector.cpp
///////////////////////////



std::vector<std::string> accounts_to_insert {
    "joe",
    "mike",
    "dave",
    "adam",
    "scott",
};



// TODO: Be careful! Right now, this will overwrite the current database!
TEST_CASE( "init db", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    // Init the db
    REQUIRE( sql->ExecuteSqlFile("db/init_db.sql") == true );

    delete sql;
}


TEST_CASE( "insert account", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    std::string email("my_email@my.example.com");
    std::string key("deadBEEF019");
    std::string salt("deAD1337");

    // Insert the accounts
    for (int i = 0; i < accounts_to_insert.size(); ++i) {
        REQUIRE(sql->InsertAccount(accounts_to_insert[i],email,key,salt) != 0);
    }

    // Make sure reinserting accounts with the same name fails
    for (int i = 0; i < accounts_to_insert.size(); ++i) {
        REQUIRE(sql->InsertAccount(accounts_to_insert[i],email,key,salt) == 0);
    }

    std::vector<std::string> accounts = sql->GetAccounts();
    // Make sure all the accounts were inserted
    REQUIRE( accounts.size() == accounts_to_insert.size());

    delete sql;
}


TEST_CASE( "insert user", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    sockaddr_in dummyClient;

    User tony(
        dummyClient,
        accounts_to_insert[0],
        17,
        1,
        "Tony",
        "Starks"
    );

    int account_id = 1;
    REQUIRE( sql->InsertUser(tony, account_id) != 0 );
    // Test to make sure users looks good
    std::vector<std::string> users = sql->GetUserList(accounts_to_insert[0]);
    REQUIRE( users.size() > 0 );
    delete sql;
}



///////////////////////////
// Utils
///////////////////////////


TEST_CASE( "Tokenfy", "[utils]" ) {
    std::vector<std::string> temp = utils::Tokenfy(std::string("This is a string"),' ');
    REQUIRE( temp.size() == 4 );
    REQUIRE( temp[0].compare("This") == 0 );
    REQUIRE( temp[3].compare("string") == 0 );

    temp = utils::Tokenfy(std::string(" abc.def.ghi "),'.');
    REQUIRE( temp.size() == 3 );
    REQUIRE( temp[0].compare(" abc") == 0 );
    REQUIRE( temp[2].compare("ghi ") == 0 );

    temp = utils::Tokenfy(std::string("....."),'.');
    REQUIRE( temp.size() == 6 );
    REQUIRE( temp[0].compare("") == 0 );
    REQUIRE( temp[5].compare("") == 0 );

    temp = utils::Tokenfy(std::string(""),'.');
    REQUIRE( temp.size() == 1 );
    REQUIRE( temp[0].compare("") == 0 );

    temp = utils::Tokenfy(std::string("askdjf kjhaskld fklj askljfh ljh"),'.');
    REQUIRE( temp.size() == 1 );
}










TEST_CASE( "SanitizeAccountName", "[utils]" ) {
    REQUIRE( utils::SanitizeAccountName("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") == false );
    REQUIRE( utils::SanitizeAccountName("!@#$%!@%@!^#$%@#$") == false );
    REQUIRE( utils::SanitizeAccountName("; drop all tables") == false );
    REQUIRE( utils::SanitizeAccountName("A") == false );
    REQUIRE( utils::SanitizeAccountName("") == false );
    REQUIRE( utils::SanitizeAccountName("        ") == false );
    REQUIRE( utils::SanitizeAccountName("my_ACCOUNT_1234   ") == false );

    REQUIRE( utils::SanitizeAccountName("my_ACCOUNT_1234") == true );


}

TEST_CASE( "CheckAccountNameLength", "[utils]" ) {
    REQUIRE( utils::CheckAccountNameLength("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") == false );
    REQUIRE( utils::CheckAccountNameLength("A") == false );
    REQUIRE( utils::CheckAccountNameLength("") == false );

    REQUIRE( utils::CheckAccountNameLength("my_ACCOUNT_1234") == true );
}


TEST_CASE( "SanitizeAlphanumeric", "[utils]" ) {
    REQUIRE( utils::SanitizeAlphanumeric("*&!@^*&#@@#$") == false );
    REQUIRE( utils::SanitizeAlphanumeric(";;;;;") == false );
    REQUIRE( utils::SanitizeAlphanumeric("||||") == false );
    REQUIRE( utils::SanitizeAlphanumeric("----") == false );
    REQUIRE( utils::SanitizeAlphanumeric("        ") == false );
    REQUIRE( utils::SanitizeAlphanumeric("my_ACCOUNT_1234   ") == false );
    REQUIRE( utils::SanitizeAlphanumeric("####") == false );

    REQUIRE( utils::SanitizeAlphanumeric("____") == true );
    REQUIRE( utils::SanitizeAlphanumeric("my_ACCOUNT_1234") == true );
}



// TODO: Create tests for all the Util functions, if possible
// TODO: Create Util functions for setting up the IP stuff, so it's easier to call here
// TODO: Create tests

// TODO: Try out some BDD tests - Catch supports them!



// TEST_CASE( "Test instantiating Server on port 7331", "[server]" ) {
//     Server * server = new Server(7331);
//     delete server;
// }




// References:
// How to use Catch:
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md