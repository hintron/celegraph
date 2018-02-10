// Filename:    SQLConnector.cpp
// Author:      Joseph DeVictoria
// Date:        Feb_29_2016
// Purpose:     SQL Connector class to handle SQLite3 queries.

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
SQLConnector::SQLConnector() : db("db/Oldentide.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE){
    std::cout << "Oldentide database \"" << db.getFilename() << "\" opened successfully." << std::endl;
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


int SQLConnector::InsertNpc(Npc n){
    std::cout << "Creating npc " << n.GetFirstname() << " " << n.GetLastname() << std::endl;

    location_t location = n.GetLocation();
    equipment_t equip = n.GetEquipment();
    stats_t stats = n.GetStats();

    // Now insert the player using the newly-created character
    std::string query_string(R"(
    INSERT INTO npcs (
        firstname,
        lastname,
        guild,
        race,
        gender,
        face,
        skin,
        profession,
        level,
        hp,
        maxhp,
        bp,
        maxbp,
        mp,
        maxmp,
        ep,
        maxep,
        strength,
        constitution,
        intelligence,
        dexterity,
        head,
        chest,
        arms,
        hands,
        legs,
        feet,
        cloak,
        necklace,
        ringone,
        ringtwo,
        righthand,
        lefthand,
        zone,
        x,
        y,
        z,
        pitch,
        yaw
    ) VALUES (
        :firstname,
        :lastname,
        :guild,
        :race,
        :gender,
        :face,
        :skin,
        :profession,
        :level,
        :hp,
        :maxhp,
        :bp,
        :maxbp,
        :mp,
        :maxmp,
        :ep,
        :maxep,
        :strength,
        :constitution,
        :intelligence,
        :dexterity,
        :head,
        :chest,
        :arms,
        :hands,
        :legs,
        :feet,
        :cloak,
        :necklace,
        :ringone,
        :ringtwo,
        :righthand,
        :lefthand,
        :zone,
        :x,
        :y,
        :z,
        :pitch,
        :yaw
    )
    )");

    try {
        SQLite::Statement query(db, query_string);
        // Identification
        query.bind(":firstname", n.GetFirstname());
        query.bind(":lastname", n.GetLastname());
        query.bind(":guild", n.GetGuild());
        query.bind(":race", n.GetRace());
        query.bind(":gender", n.GetGender());
        query.bind(":face", n.GetFace());
        query.bind(":skin", n.GetSkin());
        query.bind(":profession", n.GetProfession());
        // Stats
        query.bind(":level", stats.level);
        query.bind(":hp", stats.hp);
        query.bind(":maxhp", stats.maxhp);
        query.bind(":bp", stats.bp);
        query.bind(":maxbp", stats.maxbp);
        query.bind(":mp", stats.mp);
        query.bind(":maxmp", stats.maxmp);
        query.bind(":ep", stats.ep);
        query.bind(":maxep", stats.maxep);
        query.bind(":strength", stats.strength);
        query.bind(":constitution", stats.constitution);
        query.bind(":intelligence", stats.intelligence);
        query.bind(":dexterity", stats.dexterity);
        // Equipment
        query.bind(":head", equip.head);
        query.bind(":chest", equip.chest);
        query.bind(":arms", equip.arms);
        query.bind(":hands", equip.hands);
        query.bind(":legs", equip.legs);
        query.bind(":feet", equip.feet);
        query.bind(":cloak", equip.cloak);
        query.bind(":necklace", equip.necklace);
        query.bind(":ringone", equip.ringone);
        query.bind(":ringtwo", equip.ringtwo);
        query.bind(":righthand", equip.righthand);
        query.bind(":lefthand", equip.lefthand);
        // Location
        query.bind(":zone", n.GetZone());
        query.bind(":x", location.x);
        query.bind(":y", location.y);
        query.bind(":z", location.z);
        query.bind(":pitch", location.pitch);
        query.bind(":yaw", location.yaw);
        int rows_modified = query.exec();
        if(rows_modified < 1){
            std::cout << "Could not insert npc record! " << rows_modified << " rows were modified" << std::endl;
            return 0;
        }
        else {
            // TODO: Loop through all the items and create them?
            // inventory

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


// Inserts a new player into the database.
int SQLConnector::InsertPlayer(Player p, int account_id) {
    std::cout << "Creating player " << p.GetFirstname() << " " << p.GetLastname() << std::endl;

    location_t location = p.GetLocation();
    equipment_t equip = p.GetEquipment();
    stats_t stats = p.GetStats();
    skills_t skills = p.GetSkills();

    // TODO: Deal with these fields
    // sockaddr_in client,
    // std::string account,
    // int id,
    // int session,

    // TODO: Update active player list


    // Now insert the player using the newly-created character
    std::string query_string(R"(
    INSERT INTO players (
        account_id,
        -- Identification:
        firstname,
        lastname,
        guild,
        race,
        gender,
        face,
        skin,
        profession,
        -- Stats:
        level,
        hp,
        maxhp,
        bp,
        maxbp,
        mp,
        maxmp,
        ep,
        maxep,
        strength,
        constitution,
        intelligence,
        dexterity,
        -- Skills
        axe,
        dagger,
        unarmed,
        hammer,
        polearm,
        spear,
        staff,
        sword,
        archery,
        crossbow,
        sling,
        thrown,
        armor,
        dualweapon,
        shield,
        bardic,
        conjuring,
        druidic,
        illusion,
        necromancy,
        sorcery,
        shamanic,
        summoning,
        spellcraft,
        focus,
        armorsmithing,
        tailoring,
        fletching,
        weaponsmithing,
        alchemy,
        lapidary,
        calligraphy,
        enchanting,
        herbalism,
        hunting,
        mining,
        bargaining,
        camping,
        firstaid,
        lore,
        picklocks,
        scouting,
        search,
        stealth,
        traps,
        aeolandis,
        hieroform,
        highgundis,
        oldpraxic,
        praxic,
        runic,
        -- Equipment:
        head,
        chest,
        arms,
        hands,
        legs,
        feet,
        cloak,
        necklace,
        ringone,
        ringtwo,
        righthand,
        lefthand,
        -- Location
        zone,
        x,
        y,
        z,
        pitch,
        yaw
    ) VALUES (
        :account_id,
        -- Identification
        :firstname,
        :lastname,
        :guild,
        :race,
        :gender,
        :face,
        :skin,
        :profession,
        -- Stats
        :level,
        :hp,
        :maxhp,
        :bp,
        :maxbp,
        :mp,
        :maxmp,
        :ep,
        :maxep,
        :strength,
        :constitution,
        :intelligence,
        :dexterity,
        -- Skills
        :axe,
        :dagger,
        :unarmed,
        :hammer,
        :polearm,
        :spear,
        :staff,
        :sword,
        :archery,
        :crossbow,
        :sling,
        :thrown,
        :armor,
        :dualweapon,
        :shield,
        :bardic,
        :conjuring,
        :druidic,
        :illusion,
        :necromancy,
        :sorcery,
        :shamanic,
        :summoning,
        :spellcraft,
        :focus,
        :armorsmithing,
        :tailoring,
        :fletching,
        :weaponsmithing,
        :alchemy,
        :lapidary,
        :calligraphy,
        :enchanting,
        :herbalism,
        :hunting,
        :mining,
        :bargaining,
        :camping,
        :firstaid,
        :lore,
        :picklocks,
        :scouting,
        :search,
        :stealth,
        :traps,
        :aeolandis,
        :hieroform,
        :highgundis,
        :oldpraxic,
        :praxic,
        :runic,
        -- Equipment
        :head,
        :chest,
        :arms,
        :hands,
        :legs,
        :feet,
        :cloak,
        :necklace,
        :ringone,
        :ringtwo,
        :righthand,
        :lefthand,
        -- Location
        :zone,
        :x,
        :y,
        :z,
        :pitch,
        :yaw
    )
    )");

    try {
        SQLite::Statement query(db, query_string);
        query.bind(":account_id", account_id);
        // Identification
        query.bind(":firstname", p.GetFirstname());
        query.bind(":lastname", p.GetLastname());
        query.bind(":guild", p.GetGuild());
        query.bind(":race", p.GetRace());
        query.bind(":gender", p.GetGender());
        query.bind(":face", p.GetFace());
        query.bind(":skin", p.GetSkin());
        query.bind(":profession", p.GetProfession());
        // Stats
        query.bind(":level", stats.level);
        query.bind(":hp", stats.hp);
        query.bind(":maxhp", stats.maxhp);
        query.bind(":bp", stats.bp);
        query.bind(":maxbp", stats.maxbp);
        query.bind(":mp", stats.mp);
        query.bind(":maxmp", stats.maxmp);
        query.bind(":ep", stats.ep);
        query.bind(":maxep", stats.maxep);
        query.bind(":strength", stats.strength);
        query.bind(":constitution", stats.constitution);
        query.bind(":intelligence", stats.intelligence);
        query.bind(":dexterity", stats.dexterity);
        // Skills
        query.bind(":axe", skills.axe);
        query.bind(":dagger", skills.dagger);
        query.bind(":unarmed", skills.unarmed);
        query.bind(":hammer", skills.hammer);
        query.bind(":polearm", skills.polearm);
        query.bind(":spear", skills.spear);
        query.bind(":staff", skills.staff);
        query.bind(":sword", skills.sword);
        query.bind(":archery", skills.archery);
        query.bind(":crossbow", skills.crossbow);
        query.bind(":sling", skills.sling);
        query.bind(":thrown", skills.thrown);
        query.bind(":armor", skills.armor);
        query.bind(":dualweapon", skills.dualweapon);
        query.bind(":shield", skills.shield);
        query.bind(":bardic", skills.bardic);
        query.bind(":conjuring", skills.conjuring);
        query.bind(":druidic", skills.druidic);
        query.bind(":illusion", skills.illusion);
        query.bind(":necromancy", skills.necromancy);
        query.bind(":sorcery", skills.sorcery);
        query.bind(":shamanic", skills.shamanic);
        query.bind(":summoning", skills.summoning);
        query.bind(":spellcraft", skills.spellcraft);
        query.bind(":focus", skills.focus);
        query.bind(":armorsmithing", skills.armorsmithing);
        query.bind(":tailoring", skills.tailoring);
        query.bind(":fletching", skills.fletching);
        query.bind(":weaponsmithing", skills.weaponsmithing);
        query.bind(":alchemy", skills.alchemy);
        query.bind(":lapidary", skills.lapidary);
        query.bind(":calligraphy", skills.calligraphy);
        query.bind(":enchanting", skills.enchanting);
        query.bind(":herbalism", skills.herbalism);
        query.bind(":hunting", skills.hunting);
        query.bind(":mining", skills.mining);
        query.bind(":bargaining", skills.bargaining);
        query.bind(":camping", skills.camping);
        query.bind(":firstaid", skills.firstaid);
        query.bind(":lore", skills.lore);
        query.bind(":picklocks", skills.picklocks);
        query.bind(":scouting", skills.scouting);
        query.bind(":search", skills.search);
        query.bind(":stealth", skills.stealth);
        query.bind(":traps", skills.traps);
        query.bind(":aeolandis", skills.aeolandis);
        query.bind(":hieroform", skills.hieroform);
        query.bind(":highgundis", skills.highgundis);
        query.bind(":oldpraxic", skills.oldpraxic);
        query.bind(":praxic", skills.praxic);
        query.bind(":runic", skills.runic);
        // Equipment
        query.bind(":head", equip.head);
        query.bind(":chest", equip.chest);
        query.bind(":arms", equip.arms);
        query.bind(":hands", equip.hands);
        query.bind(":legs", equip.legs);
        query.bind(":feet", equip.feet);
        query.bind(":cloak", equip.cloak);
        query.bind(":necklace", equip.necklace);
        query.bind(":ringone", equip.ringone);
        query.bind(":ringtwo", equip.ringtwo);
        query.bind(":righthand", equip.righthand);
        query.bind(":lefthand", equip.lefthand);
        // Location
        query.bind(":zone", p.GetZone());
        query.bind(":x", location.x);
        query.bind(":y", location.y);
        query.bind(":z", location.z);
        query.bind(":pitch", location.pitch);
        query.bind(":yaw", location.yaw);
        int rows_modified = query.exec();

        if(rows_modified < 1){
            std::cout << "Could not insert player record! " << rows_modified << " rows were modified" << std::endl;
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




int SQLConnector::InsertItem(Item i) {
    std::cout << "Creating item " << i.GetName() << std::endl;

    std::string query_string(R"(
    INSERT INTO items (
        name,
        location,
        weight,
        equip,
        use,
        x,
        y
    ) VALUES (
        :name,
        :location,
        :weight,
        :equip,
        :use,
        :x,
        :y
    )
    )");

    try {
        SQLite::Statement query(db, query_string);
        query.bind(":name", i.name);
        query.bind(":location", i.location);
        query.bind(":weight", i.weight);
        query.bind(":equip", i.equip);
        query.bind(":use", i.use);
        query.bind(":x", i.x);
        query.bind(":y", i.y);
        int rows_modified = query.exec();
        if(rows_modified < 1){
            std::cout << "Could not insert item record! " << rows_modified << " rows were modified" << std::endl;
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




std::vector<std::string> SQLConnector::GetPlayerList(std::string accountname) {
    std::vector<std::string> players;

    std::string cmd("SELECT firstname, lastname, accountname FROM view_players WHERE accountname = ? ORDER BY lastname");
    SQLite::Statement query(db, cmd);
    query.bind(1, accountname);

    while (query.executeStep()) {
        std::string name = query.getColumn(0);
        std::string lastname = query.getColumn(1);
        name.append(" ").append(lastname);
        players.push_back(name);
    }
    return players;
}

std::vector<Npc> SQLConnector::GetNpcs() {
    std::vector<Npc> npcs;
    // TODO: Get equipment, stats, location
    std::string cmd(R"(SELECT
        id,
        firstname,
        lastname,
        guild,
        race,
        gender,
        face,
        skin,
        zone,
        profession
        FROM npcs ORDER BY lastname
    )");

    SQLite::Statement query(db, cmd);

    // TODO: Get the real values
    equipment_t empty_equipment;
    stats_t empty_stats;
    location_t empty_location;
    while (query.executeStep()) {
        Npc temp(
            query.getColumn(0),
            query.getColumn(1),
            query.getColumn(2),
            query.getColumn(3),
            query.getColumn(4),
            query.getColumn(5),
            query.getColumn(6),
            query.getColumn(7),
            query.getColumn(8),
            query.getColumn(9),
            empty_equipment,
            empty_stats,
            empty_location
        );
        npcs.push_back(temp);
    }
    return npcs;
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
