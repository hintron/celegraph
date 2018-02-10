// Filename:    UnitTest.cpp
// Author:      Joseph DeVictoria
// Date:        September_24_2016
// Purpose:     A non-exhaustive unit testing platform to quickly enable functions.

// Load the testing framework (do it ABOVE the catch include!)
#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include "AdminShell.h"
#include "GameState.h"
#include "Item.h"
#include "Character.h"
#include "Player.h"
#include "Npc.h"
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
//       get characters
//       get players
//       get npcs
// TODO: get item
//       insert account
//       insert character
//       insert player
//       insert npc
// TODO: insert item
// TODO: delete account - test cascade deletes
// TODO: delete character - test cascade deletes
// TODO: delete player
// TODO: delete npc
// TODO: delete item
// TODO: update account - test cascade updates
// TODO: update player
// TODO: update npc
// TODO: update character - test cascade updates
// TODO: update item
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
    REQUIRE( sql->ExecuteSqlFile("db/InitializeDb.sql") == true );

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


TEST_CASE( "insert player", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    sockaddr_in dummyClient;
    equipment_t dummyEquipment = {};
    stats_t dummyStats = {};
    skills_t dummySkills = {};
    location_t dummyLocation = {};

    Player tony(
        dummyClient,
        accounts_to_insert[0],
        17,
        1,
        dummySkills,
        "Tony",
        "Starks",
        "Marvel",
        "Human",
        "Male",
        "Mustached",
        "White",
        "zone1",
        "Electrical Engineer",
        dummyEquipment,
        dummyStats,
        dummyLocation
    );

    int account_id = 1;
    REQUIRE( sql->InsertPlayer(tony, account_id) != 0 );
    // Test to make sure players looks good
    std::vector<std::string> players = sql->GetPlayerList(accounts_to_insert[0]);
    REQUIRE( players.size() > 0 );
    delete sql;
}



TEST_CASE( "insert npcs", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    equipment_t dummyEquipment = {};
    location_t dummyLocation = {};
    stats_t dummyStats = {};

    Npc griphook(
        0,
        "Griphook",
        "",
        "Gringotts",
        "Goblin",
        "Male",
        "Normal",
        "Pale",
        "zone1",
        "Banker",
        dummyEquipment,
        dummyStats,
        dummyLocation
    );

    int npc_id = sql->InsertNpc(griphook);
    REQUIRE( npc_id != 0 );

    // Check to see if id getter and setter works
    griphook.SetId(npc_id);
    REQUIRE( griphook.GetId() == npc_id );

    std::vector<Npc> npcs = sql->GetNpcs();
    REQUIRE( npcs.size() > 0 );
    REQUIRE( npcs.at(0).GetFirstname() == "Griphook" );

    delete sql;
}




TEST_CASE( "insert items", "[sql]" ) {
    SQLConnector* sql = new SQLConnector();
    // Character dummyCharacter;

    Item sting(
        1,
        "Sting",
        "dummy_location",
        "dummy_weight",
        true,
        false,
        17,
        17
    );

    int item_id = sql->InsertItem(sting);
    REQUIRE( item_id != 0 );

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