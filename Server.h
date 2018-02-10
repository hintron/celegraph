// This server is a UDP Server which will receive and handle all
// packets sent from the client.

#ifndef CELEGRAPH_SERVER_H
#define CELEGRAPH_SERVER_H

#include "Packets.h"
#include "User.h"
#include "SQLConnector.h"
#include <arpa/inet.h>
#include <set>
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

// forward declaration so AdminShell and Server can see each other
class AdminShell;
class GameState;

class Server{
    public:
        Server(int port);
        ~Server();
        void Run();
        int GetPacketQueueSize();
        void BroadcastToConnections(std::string msg, std::string fromUser);
        void SendMessageToConnection(std::string msg, std::string fromUser, std::string toUser);
    private:
        int sockfd;
        SQLConnector * sql;
        GameState * gameState;
        AdminShell * adminshell;
        std::queue<packets::packet_t> packetQueue;
        // Session to socket mapping
        std::map<int, sockaddr_in> activeConnections;

        // Wrap all globals accessed in WorkerThread with mutexes
        std::mutex gameStateMutex;
        std::mutex packetQueueMutex;
        std::mutex activeConnectionsMutex;
        // NOTE: Sockets are atomic, so they are thread safe
        // https://stackoverflow.com/questions/1981372/are-parallel-calls-to-send-recv-on-the-same-socket-valid

        std::condition_variable packetQueueSyncVar;

        void WorkerThread(int id);
        // void StatisticsThread();
        void GenericHandler(msgpack::object_handle *data, sockaddr_in *client);
        void AckHandler(msgpack::object_handle *data, sockaddr_in *client);
        void ConnectHandler(msgpack::object_handle *data, sockaddr_in *client);
        void DisconnectHandler(msgpack::object_handle *data, sockaddr_in *client);
        void ListUsersHandler(msgpack::object_handle *data, sockaddr_in *client);
        void SelectUserHandler(msgpack::object_handle *data, sockaddr_in *client);
        void DeleteUserHandler(msgpack::object_handle *data, sockaddr_in *client);
        void CreateUserHandler(msgpack::object_handle *data, sockaddr_in *client);
        void InitializeGameHandler(msgpack::object_handle *data, sockaddr_in *client);
        void UpdatePcHandler(msgpack::object_handle *data, sockaddr_in *client);
        void UpdateNpcHandler(msgpack::object_handle *data, sockaddr_in *client);
        void SendPlayerCommandHandler(msgpack::object_handle *data, sockaddr_in *client);
        void SendPlayerActionHandler(msgpack::object_handle *data, sockaddr_in *client);
        void SendServerActionHandler(msgpack::object_handle *data, sockaddr_in *client);
        void UnityHandler(msgpack::object_handle *data, sockaddr_in *client);
};

#endif //CELEGRAPH_SERVER_H
