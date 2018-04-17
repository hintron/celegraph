#include "AdminShell.h"
#include "Server.h"
#include "Utils.h"
#include "Packets.h"
#include <iostream>
#include <sstream>
#include <string>

AdminShell::AdminShell(Server * server, SQLConnector * sql, ServerState * server_state) {
    this->server = server;
    this->sql = sql;
    this->server_state = server_state;
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
            std::cout << "CelegraphAdmin@" << serverHostname << ": ";
            getline(std::cin,adminCommand);
	    }while(adminCommand.empty());
        std::vector<std::string> adminTokens = utils::Tokenfy(adminCommand, ' ');
        if (adminTokens[0] == "/shutdown") {
            std::cout << "Celegraph Server is shutting down..." << std::endl;
            exit(EXIT_SUCCESS);
            return;
        }
        else if (adminTokens[0] == "/list") {
            if (adminTokens.size() == 2) {
                std::cout << adminTokens[1];
                if (adminTokens[1] == "accounts") {
                    sql->ListAccounts();
                }
                if (adminTokens[1] == "users") {
                    std::vector<User> users = server_state->getUsers();
                    for (int i = 0; i < users.size(); i++) {
                        User temp = users.at(i);
                        std::cout << temp.GetFirstname() << " " << temp.GetLastname() << std::endl;
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
                    std::cout << "LISTUSERS (" << (int) packets::LISTUSERS << "): " << sizeof(packets::Listusers) << std::endl;
                    std::cout << "SELECTUSER (" << (int) packets::SELECTUSER << "): " << sizeof(packets::Selectuser) << std::endl;
                    std::cout << "DELETEUSER (" << (int) packets::DELETEUSER << "): " << sizeof(packets::Deleteuser) << std::endl;
                    std::cout << "CREATEUSER (" << (int) packets::CREATEUSER << "): " << sizeof(packets::Createuser) << std::endl;
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
        // Broadcast to all currently connected users
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
            // TODO Actually instantiate a game user server side
            // server->BroadcastToConnections(std::string(adminCommand.c_str()+3), std::string("admin"));
        }
        else {
            PrintUsage();
        }
    }
}

void AdminShell::PrintUsage() {
    std::cout << "Dedicated Server Admin Usage:" << std::endl;
    std::cout << "/shutdown                 = Shuts down the server." << std::endl;
    std::cout << "/list <var>               = Lists all possible <var>s. [accounts, users, packets, packetqueue(pq)]." << std::endl;
    std::cout << "/db <query>               = Runs a given sql query on the sqlite3 database." << std::endl;
    std::cout << "/h <message>              = Broadcasts <message> to all users." << std::endl;
    std::cout << "/h <client_id> <message>  = Sends <message> to user with <client_id> = ." << std::endl;
}

void AdminShell::PrintLogo() {
    std::string logo_str(R"(
   _____     _                            _
  / ____|   | |                          | |
 | |     ___| | ___  __ _ _ __ __ _ _ __ | |__
 | |    / _ \ |/ _ \/ _` | '__/ _` | '_ \| '_ \
 | |___|  __/ |  __/ (_| | | | (_| | |_) | | | |
  \_____\___|_|\___|\__, |_|  \__,_| .__/|_| |_|
                     __/ |         | |
                    |___/          |_|

Welcome to the Celegraph server shell!
)");
    std::cout << logo_str << std::endl;
}
