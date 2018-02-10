// Filename:    AdminShell.cpp
// Author:      Joseph DeVictoria
// Date:        Mar_6_2016
// Purpose:     Provide an interface for the administrator to interact with
//              the server.  This class will handle all server admin commands.

#include "AdminShell.h"
#include "Server.h"
#include "Utils.h"
#include "Packets.h"
#include <iostream>
#include <sstream>
#include <string>

AdminShell::AdminShell(Server * server, SQLConnector * sql, GameState * gameState) {
    this->server = server;
    this->sql = sql;
    this->gameState = gameState;
    gethostname(serverHostname, HOST_NAME_MAX);
}

AdminShell::~AdminShell() {
    return;
}

void AdminShell::operator()() {
    Run();
}

void AdminShell::Run() {
    std::string adminCommand;
    std::cout << "Starting Server Administrator Shell.\n";
    PrintLogo();
    while(true) {
	    do
	    {
            std::cout << "OldentideAdmin@" << serverHostname << ": ";
            getline(std::cin,adminCommand);
	    }while(adminCommand.empty());
        std::vector<std::string> adminTokens = utils::Tokenfy(adminCommand, ' ');
        if (adminTokens[0] == "/shutdown") {
            std::cout << "Oldentide Dedicated Server is shutting down..." << std::endl;
            exit(EXIT_SUCCESS);
            return;
        }
        else if (adminTokens[0] == "/list") {
            if (adminTokens.size() == 2) {
                std::cout << adminTokens[1];
                if (adminTokens[1] == "accounts") {
                    sql->ListAccounts();
                }
                if (adminTokens[1] == "players") {
                    std::vector<Player> players = gameState->getPlayers();
                    for (int i = 0; i < players.size(); i++) {
                        Player temp = players.at(i);
                        std::cout << temp.GetFirstname() << " " << temp.GetLastname() << std::endl;
                    }
                }
                if (adminTokens[1] == "npcs") {
                    std::cout << "NPCS" << std::endl;
                    std::vector<Npc> npcs = gameState->getNPCs();
                    for (int i = 0; i < npcs.size(); i++) {
                        Npc temp = npcs.at(i);
                        std::cout << temp.GetId() << std::endl;
                    }
                }
                if (adminTokens[1] == "packetqueue" || adminTokens[1] == "pq") {
                    int size = server->GetPacketQueueSize();
                    std::cout << "Instantaneous packetQueue size: " << size << std::endl;
                }
                if (adminTokens[1] == "packets") {
                    std::cout << "\nMAX Packet Size (w/msgpack):" << packets::PACKET_MAX_SIZE << std::endl;;
                    std::cout << "GENERIC (" << (int) packets::GENERIC << "): " << sizeof(packets::Generic) << std::endl;
                    std::cout << "ACK (" << (int) packets::ACK << "): " << sizeof(packets::Ack) << std::endl;
                    std::cout << "CONNECT (" << (int) packets::CONNECT << "): " << sizeof(packets::Connect) << std::endl;
                    std::cout << "DISCONNECT (" << (int) packets::DISCONNECT << "): " << sizeof(packets::Disconnect) << std::endl;
                    std::cout << "ERROR (" << (int) packets::ERROR << "): " << sizeof(packets::Error) << std::endl;
                    std::cout << "LISTCHARACTERS (" << (int) packets::LISTCHARACTERS << "): " << sizeof(packets::Listcharacters) << std::endl;
                    std::cout << "SELECTCHARACTER (" << (int) packets::SELECTCHARACTER << "): " << sizeof(packets::Selectcharacter) << std::endl;
                    std::cout << "DELETECHARACTER (" << (int) packets::DELETECHARACTER << "): " << sizeof(packets::Deletecharacter) << std::endl;
                    std::cout << "CREATECHARACTER (" << (int) packets::CREATECHARACTER << "): " << sizeof(packets::Createcharacter) << std::endl;
                    std::cout << "INITIALIZEGAME (" << (int) packets::INITIALIZEGAME << "): " << sizeof(packets::Initializegame) << std::endl;
                    std::cout << "UPDATEPC (" << (int) packets::UPDATEPC << "): " << sizeof(packets::Updatepc) << std::endl;
                    std::cout << "UPDATENPC (" << (int) packets::UPDATENPC << "): " << sizeof(packets::Updatenpc) << std::endl;
                    std::cout << "SENDPLAYERCOMMAND (" << (int) packets::SENDPLAYERCOMMAND << "): " << sizeof(packets::Sendplayercommand) << std::endl;
                    std::cout << "SENDSERVERCOMMAND (" << (int) packets::SENDSERVERCOMMAND << "): " << sizeof(packets::Sendservercommand) << std::endl;
                    std::cout << "SENDPLAYERACTION (" << (int) packets::SENDPLAYERACTION << "): " << sizeof(packets::Sendplayeraction) << std::endl;
                    std::cout << "SENDSERVERACTION (" << (int) packets::SENDSERVERACTION << "): " << sizeof(packets::Sendserveraction) << std::endl;
                }
            }
            else {
                PrintUsage();
            }
        }
        else if (adminTokens[0] == "/db") {
            std::string cmd = adminCommand.erase(0,4);
            sql->Execute(cmd);
        }
        // Broadcast to all currently connected players
        else if (adminTokens[0] == "/h") {
            server->BroadcastToConnections(std::string(adminCommand.c_str()+3), std::string("admin"));
        }
        // e.g. /s 1 What's up, Client 1?
        // e.g. /s admin What's up, admin?
        else if (adminTokens[0] == "/s") {
            const char *msgPointer = adminCommand.c_str()+adminTokens[0].length()+1+adminTokens[1].length()+1;
            server->SendMessageToConnection(std::string(msgPointer), std::string("admin"), std::string(adminTokens[1]));
        }
        else if (adminTokens[0] == "/create") {
            // TODO Actually instantiate a game character server side
            // server->BroadcastToConnections(std::string(adminCommand.c_str()+3), std::string("admin"));
        }
        else {
            PrintUsage();
        }
    }
}

void AdminShell::PrintUsage() {
    std::cout << "Dedicated Server Admin Usage:" << std::endl;
    std::cout << "/shutdown              = Shuts down the server." << std::endl;
    std::cout << "/list <var>            = Lists all entities of given <var> on server, where <var> is [players, npcs, accounts, packets, packetqueue(pq)]." << std::endl;
    std::cout << "/db <query>            = Runs a given sql query on the sqlite3 database." << std::endl;
    std::cout << "/h <message>           = Broadcasts a message to all users." << std::endl;
    std::cout << "/h <client> <message>  = Broadcasts a message to all users." << std::endl;
    std::cout << "/create <gameObjectType> <id> = Manually creates a game object." << std::endl;
}

void AdminShell::PrintLogo() {
    std::cout << "  ____           ___   _____         _____  _____  ___   _____" << std::endl;
    std::cout << " /    \\  |      |   \\  |      |   |    |      |    |  \\  |" << std::endl;
    std::cout << "/      \\ |      |    \\ |      |\\  |    |      |    |   \\ |" << std::endl;
    std::cout << "|      | |      |    | |___   | \\ |    |      |    |   | |___" << std::endl;
    std::cout << "\\      / |      |   /  |      |  \\|    |      |    |   / |" << std::endl;
    std::cout << " \\____/  |_____ |__/   |____  |   \\    |    __|__  |__/  |____" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "                              |" << std::endl;
    std::cout << "                             / \\" << std::endl;
    std::cout << "                            /\\_/\\" << std::endl;
    std::cout << "                           / | | \\" << std::endl;
    std::cout << "                           \\ |_| /" << std::endl;
    std::cout << "                            \\/ \\/" << std::endl;
    std::cout << "                             \\ /" << std::endl;
    std::cout << "                              |" << std::endl;
    std::cout << " " << std::endl;
}
