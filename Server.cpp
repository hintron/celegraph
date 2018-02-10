#include "Server.h"
#include "AdminShell.h"
#include "ServerState.h"
#include <cstring>
#include <iostream>
#include <iterator>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <limits.h>
#include <msgpack.hpp>
#include <sstream>
#include "Utils.h"
#include <queue>
#include <mutex>
#include <chrono>

// TODO: Create a helper function to reduce try/catch code with messagepack

Server::Server(int port) {
    sql = new SQLConnector();
    serverState = new ServerState(this, sql);
    adminshell = new AdminShell(this, sql, serverState);

    // Create server address struct.
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    // Create socket for IP, UDP normal protocol.
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        std::cout << "Cannot create socket..." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind socket to a port.
    if ((bind(sockfd, (struct sockaddr *)&server, sizeof(server))) < 0) {
        std::cout << "Cannot bind socket..." << std::endl;
        exit(EXIT_FAILURE);
    };

    std::cout << "Server socket: " << utils::GetIpAndPortFromSocket(&server) <<  std::endl;
}

Server::~Server() {
    delete sql;
    delete serverState;
    delete adminshell;
}

void Server::Run() {
    std::thread shell(*adminshell);

    // TODO: There should be 1 active thread per logical core to prevent thrashing
    int NUM_WORKER_THREADS = 4;

    // Initialize all the threads
    for (int i = 0; i < NUM_WORKER_THREADS; ++i) {
        // Start a thread on a Server member function
        // Funky syntax; server object needs to be second param
        // See http://stackoverflow.com/questions/10673585/start-thread-with-member-function
        std::thread t(&Server::WorkerThread, this, i);
        t.detach(); // Detach the threads so we don't need to join them manually
    }

    // // Set up a statistics thread, to monitor the server
    // std::thread statThread(&Server::StatisticsThread, this);
    // statThread.detach(); // Detach the threads so we don't need to join them manually

    bool listen = true;
    while(listen) {
        packets::packet_t p;

        // Wait for a packet
        utils::ReceivePacketFrom(sockfd, p.data, &(p.source));

        // Put it in the packetQueue
        packetQueueMutex.lock();
        packetQueue.push(p);
        packetQueueMutex.unlock();
        // Use a condition variable to notify the next worker thread to wake up
        // https://stackoverflow.com/questions/3513045/conditional-variable-vs-semaphore
        packetQueueSyncVar.notify_one();
    }

    shell.join();
}


void Server::WorkerThread(int id) {
    while(1){
        packets::packet_t packet;

        // The following 2 lines will lock, release lock, and block thread
        // until notify_one() is called after a packet arrives
        // http://en.cppreference.com/w/cpp/thread/condition_variable/wait
        std::unique_lock<std::mutex> ul(packetQueueMutex);
        packetQueueSyncVar.wait(ul);
        // std::cout << "Worker thread " << id << " handling the request!" << std::endl;
        // Retrieve a packet from the queue
        // create a copy of the packet
        packet = packetQueue.front();
        packetQueue.pop();
        packetQueueMutex.unlock();

        msgpack::object_handle deserialized_data;
        uint8_t packetType;
        deserialized_data = utils::GetDataFromPacket(packet.data, &packetType);

        switch (packetType) {
            case false:
                utils::SendErrorTo(sockfd, std::string("Invalid packetType. Ignoring packet."), &(packet.source));
                break;
            case packets::GENERIC:
                GenericHandler(&deserialized_data, &(packet.source));
                break;
            case packets::ACK:
                AckHandler(&deserialized_data, &(packet.source));
                break;
            case packets::CONNECT:
                ConnectHandler(&deserialized_data, &(packet.source));
                break;
            case packets::DISCONNECT:
                DisconnectHandler(&deserialized_data, &(packet.source));
                break;
            case packets::LISTUSERS:
                ListUsersHandler(&deserialized_data, &(packet.source));
                break;
            case packets::SELECTUSER:
                SelectUserHandler(&deserialized_data, &(packet.source));
                break;
            case packets::DELETEUSER:
                DeleteUserHandler(&deserialized_data, &(packet.source));
                break;
            case packets::CREATEUSER:
                CreateUserHandler(&deserialized_data, &(packet.source));
                break;
            case packets::INITIALIZEGAME:
                InitializeGameHandler(&deserialized_data, &(packet.source));
                break;
            case packets::UPDATEPC:
                UpdatePcHandler(&deserialized_data, &(packet.source));
                break;
            case packets::UPDATENPC:
                UpdateNpcHandler(&deserialized_data, &(packet.source));
                break;
            case packets::SENDPLAYERCOMMAND:
                SendPlayerCommandHandler(&deserialized_data, &(packet.source));
                break;
            case packets::SENDPLAYERACTION:
                SendPlayerActionHandler(&deserialized_data, &(packet.source));
                break;
            case packets::SENDSERVERACTION:
                SendServerActionHandler(&deserialized_data, &(packet.source));
                break;
            case packets::UNITY:
                UnityHandler(&deserialized_data, &(packet.source));
                break;
            default:
                std::stringstream errSs;
                errSs << "Received unknown packet of type " << packetType;
                utils::SendErrorTo(sockfd, errSs.str(), &(packet.source));
                break;
        }
    }
}

// TODO: This can be used to eventually log things to a file
// OR, this thread could be in charge of sending PING requests to all
// actively connected users, and if a ACK request is not sent in a timely
// fashion (10 seconds?), then they are considered disconnected.
// // Print out the size of the packetQueue every so often
// void Server::StatisticsThread() {
//     while(1){
//         // Get the size
//         int size = GetPacketQueueSize();
//         // Print out the size of the packet queue
//         std::cout << "packetQueue size: " << size << std::endl;
//         std::this_thread::sleep_for(std::chrono::milliseconds(200));
//     }
// }

int Server::GetPacketQueueSize() {
    packetQueueMutex.lock();
    int size = packetQueue.size();
    packetQueueMutex.unlock();
    return size;
}

void Server::BroadcastToConnections(std::string msg, std::string user){
    // For each connected client, send a SendServerCommand packet!
    bool isAdmin = false;
    std::string broadcast;
    if(user == "admin"){
        isAdmin = true;
        broadcast = ">>>BROADCAST Oldentide Admin: " + msg + " <<<";
    }
    else {
        broadcast = ">>>BROADCAST Client " + user + ": " + msg + " <<<";
        // Print the broadcast so the server admin can see it as well
        std::cout << broadcast << std::endl;
    }
    // std::cout << "Broadcasting to sessions:" << std::endl;


    std::lock_guard<std::mutex> lk(activeConnectionsMutex);
    for (std::map<int, sockaddr_in>::iterator it = activeConnections.begin(); it != activeConnections.end(); ++it){
        // Don't broadcast to the originator of the message
        if(!isAdmin && it->first == std::stoi(user)){
            continue;
        }
        // std::cout << it->first;

        packets::Sendservercommand packet;
        packet.sessionId = it->first;
        packet.packetId = 0; // n/a
        packet.command = broadcast;
        std::stringstream buffer;
        msgpack::pack(buffer, packet);
        utils::SendDataTo(sockfd, &buffer, packets::SENDSERVERCOMMAND, &(it->second));
    }
}


void Server::SendMessageToConnection(std::string msg, std::string fromUser, std::string toUser){
    bool isAdmin = false;
    std::string formattedMsg;

    // Format the message
    if(fromUser == "admin"){
        formattedMsg = ">>>MESSAGE Oldentide Admin: " + msg + " <<<";
    }
    else {
        formattedMsg = ">>>MESSAGE Client " + fromUser + ": " + msg + " <<<";
    }

    // Figure out who to send the message to
    if(toUser == "admin"){
        // If message is sent to the admin, simply print to the admin shell
        isAdmin = true;
        std::cout << formattedMsg << std::endl;
    }
    else {
        // user should be an int
        int sessionId;
        try {
            sessionId = std::stoi(toUser);
        } catch (const std::exception& e) {
            std::string err("Failed to convert '" + toUser + "'' to a valid sessionId! Ignoring SendMessageToConnection() request from '" + fromUser + "'");
            std::cout << err << std::endl;
            // utils::SendErrorTo(sockfd, err, client);
            return;
        }

        std::lock_guard<std::mutex> lk(activeConnectionsMutex);
        std::map<int, sockaddr_in>::iterator connection = activeConnections.find(sessionId);
        if(connection == activeConnections.end()) {
            std::cout << "Could not deliver message to session '" << toUser << ": does not exist" << std::endl;
            // TODO: Send "could not deliver" error message?
            // utils::SendErrorTo(sockfd, err, client);
            return;
        }

        // Send the message to the user
        packets::Sendservercommand packet;
        packet.sessionId = sessionId;
        packet.packetId = 0; // n/a
        packet.command = formattedMsg;
        std::stringstream buffer;
        msgpack::pack(buffer, packet);
        utils::SendDataTo(sockfd, &buffer, packets::SENDSERVERCOMMAND, &(connection->second));
    }
}




// Invisible packet case, simply ignore.  We don't want the client to be able to send a generic packet...
void Server::GenericHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Generic packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        // Quit early if invalid session
        std::cout << "Invalid session. Not doing anything" << std::endl;
        return;
    }
}

// Respond to any packet that does not have an associated server action.  Those other packets will be acked by response.
void Server::AckHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Ack packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        // Quit early if invalid session
        std::cout << "Invalid session. Not doing anything" << std::endl;
        return;
    }
}

// Connect a host to the server by generating a session for it, and adding it to the serverstate sessions.
void Server::ConnectHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Connect packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }


    // Generate the new sessionId
    serverStateMutex.lock();
    int newSession = serverState->GenerateSession(packet.sessionId);
    serverStateMutex.unlock();
    // Save a copy of the connection information
    activeConnectionsMutex.lock();
    activeConnections[newSession] = *client;
    activeConnectionsMutex.unlock();
    // TODO: Figure out a way to disconnect clients if unresponsive

    packets::Connect returnPacket;
    returnPacket.sessionId = newSession;
    returnPacket.packetId = 0;
    // Use MessagePack to serialize data
    std::stringstream buffer;
    msgpack::pack(buffer, returnPacket);

    // Send the packet
    utils::SendDataTo(sockfd, &buffer, packets::CONNECT, client);

    std::cout << "\nNew connection started, session id " << returnPacket.sessionId << " sent to client!" << std::endl;
}

// Remove the session for a given user, effectively disconnecting it from the server.
void Server::DisconnectHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Disconnect packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        // Quit early if invalid session
        std::cout << "Invalid session. Not doing anything" << std::endl;
        return;
    }
    serverState->DisconnectSession(packet.sessionId);
}

void Server::ListUsersHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Listusers packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        // Quit early if invalid session
        utils::SendErrorTo(sockfd, std::string("Invalid session"), client);
        return;
    }
    std::string account = serverState->GetSessionAccountName(packet.sessionId);
    std::cout << "Account: " << account << " requested their user list." << std::endl;

    // TODO: Need to get an actual list of the player's users

    packets::Listusers returnPacket;
    returnPacket.sessionId = packet.sessionId;
    // TODO: return the same packetId that it came with?
    returnPacket.packetId = packet.packetId;
    // TODO: For testing only
        returnPacket.users.push_back(std::string("Grapthar")); // Test
        returnPacket.users.push_back(std::string("Harry Potter")); // Test
        returnPacket.users.push_back(std::string("Willy Wonka")); // Test
    // Use MessagePack to serialize data
    std::stringstream buffer;
    msgpack::pack(buffer, returnPacket);
    // Send the packet
    utils::SendDataTo(sockfd, &buffer, packets::LISTUSERS, client);
}

void Server::SelectUserHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Selectuser packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        utils::SendErrorTo(sockfd, std::string("Invalid session"), client);
        return;
    }
    std::cout << "TODO: Need to select user " << packet.user << std::endl;
    // TODO: Select the requested user
    serverState->SelectPlayer(packet.sessionId);
}

void Server::DeleteUserHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "TODO: Delete User handler!" << std::endl;
}

void Server::CreateUserHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Createuser packet;
    // Wrap this in a try catch, so bad cast doesn't crash the whole server
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }

    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        utils::SendErrorTo(sockfd, std::string("Invalid session"), client);
        return;
    }

    // TODO:
    std::cout << "TODO: Create user " << packet.lastName << ", " <<  packet.firstName << std::endl;
}

void Server::InitializeGameHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "InitializeGameHandler!" << std::endl;
}

void Server::UpdatePcHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "UpdatePcHandler!" << std::endl;
}

void Server::UpdateNpcHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "UpdateNpcHandler!" << std::endl;
}

void Server::SendPlayerCommandHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    packets::Sendplayercommand packet;
    try {
        deserialized_data->get().convert(packet);
    } catch (const std::exception& e) {
        utils::SendErrorTo(sockfd, std::string("Failed to convert/cast msgpack object"), client);
        return;
    }
    std::lock_guard<std::mutex> lk(serverStateMutex);
    if(!serverState->VerifySession(packet.sessionId)){
        utils::SendErrorTo(sockfd, std::string("Invalid session"), client);
        return;
    }

    // TODO: Get active session stuff working
    // if (serverState->VerifyActiveSession(packet.sessionId)) {
        serverState->PlayerCommand(packet.command, packet.sessionId);
    // }
    // else {
    //     std::cout << "Nonactive session requested to send a player command..." << packet.sessionId << std::endl;
    // }

    std::stringstream server_response_s;
    server_response_s << "Command successfully received";

    packets::Sendplayercommand returnPacket;
    returnPacket.sessionId = packet.sessionId;
    returnPacket.packetId = packet.packetId;
    returnPacket.command = server_response_s.str();

    std::stringstream buffer;
    msgpack::pack(buffer, returnPacket);
    utils::SendDataTo(sockfd, &buffer, packets::SENDPLAYERCOMMAND, client);
}

void Server::SendPlayerActionHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "SendPlayerActionHandler!" << std::endl;
}

void Server::SendServerActionHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "SendServerActionHandler!" << std::endl;
}


void Server::UnityHandler(msgpack::object_handle * deserialized_data, sockaddr_in *client) {
    std::cout << "UnityHandler!" << std::endl;
}