// UDP Packet Struct Definitions.

#ifndef CELEGRAPH_PACKETS_H
#define CELEGRAPH_PACKETS_H

#include <msgpack.hpp>


namespace packets {
    // Currently, we can have up to 255 packet types
    // Enumerate list of packet types (One-hot encoded)
    // TODO: Create some kind of packet that can sync the client and server packet expectations?
    enum PTYPE {
        // Reserve 0 for null/uninitialized
        GENERIC = 1,
        ACK = 2,
        CONNECT = 3,
        DISCONNECT = 4,
        ERROR = 5,
        // CREATEACCOUNT = 6,
        // LOGIN = 7,
        LISTUSERS = 8,
        SELECTUSER = 9,
        DELETEUSER = 10,
        CREATEUSER = 11,
        INITIALIZEGAME = 12,
        UPDATEPC = 13,
        UPDATENPC = 14,
        SENDPLAYERCOMMAND = 15,
        SENDSERVERCOMMAND = 16,
        SENDPLAYERACTION = 17,
        SENDSERVERACTION = 18,
        UNITY = 19
    };


    // Use this to send enums with no specific number mappings over messagepack - currently not needed
    // MSGPACK_ADD_ENUM(PTYPE);

    // Define packet maximums
    // Generally, to avoid packet fragmentation, it is suggested to keep it to around 512 for maximum leniency,
    // though they say you can go up to about 1400 for general use.
    // See http://stackoverflow.com/questions/2862071/how-large-should-my-recv-buffer-be-when-calling-recv-in-the-socket-library
    // See http://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet
    const uint16_t PACKET_MAX_SIZE = 512;
    const uint16_t PACKET_HEADER_SIZE = 3;
    const uint16_t MSGPCK_MAX_PAYLOAD_SIZE = PACKET_MAX_SIZE - PACKET_HEADER_SIZE;


    typedef struct {
        char data[PACKET_MAX_SIZE];
        sockaddr_in source;
    } packet_t;


    class Generic {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string message;
        MSGPACK_DEFINE(packetId, sessionId, message);
    };

    class Ack {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Connect {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Disconnect {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };


    class Error {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string errorMsg;
        // TODO: Implement error number system
        // int32_t errorNumber;
        MSGPACK_DEFINE(packetId, sessionId, errorMsg);
    };

    class Listusers {
    public:
        int32_t packetId;
        int32_t sessionId;
        // Array of users!
        std::vector<std::string> users;
        MSGPACK_DEFINE(packetId, sessionId, users);
    };

    class Selectuser {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string user;
        MSGPACK_DEFINE(packetId, sessionId, user);
    };

    class Deleteuser {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string user;
        MSGPACK_DEFINE(packetId, sessionId, user);
    };

    class Createuser {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string firstName;
        std::string lastName;

        std::map<std::string, uint32_t> attributes;

        MSGPACK_DEFINE(
            packetId,
            sessionId,
            firstName,
            lastName,
            attributes
        );
    };

    class Initializegame {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Updatepc {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string firstName;
        std::string lastName;
        std::string race;
        std::string gender;
        std::string profession;
        uint32_t level;
        uint32_t hp;
        uint32_t bp;
        uint32_t mp;
        uint32_t ep;
        float x;
        float y;
        float z;
        float pitch;
        float yaw;
        MSGPACK_DEFINE(packetId, sessionId,
            firstName,
            lastName,
            race,
            gender,
            profession,
            level,
            hp,
            bp,
            mp,
            ep,
            x,
            y,
            z,
            pitch,
            yaw
        );
    };

    class Updatenpc {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Sendplayercommand {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string command;
        MSGPACK_DEFINE(packetId, sessionId, command);
    };

    class Sendservercommand {
    public:
        int32_t packetId;
        int32_t sessionId;
        std::string command;
        MSGPACK_DEFINE(packetId, sessionId, command);
    };

    class Sendplayeraction {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Sendserveraction {
    public:
        int32_t packetId;
        int32_t sessionId;
        MSGPACK_DEFINE(packetId, sessionId);
    };

    class Unity {
    public:
        int32_t packetId;
        int32_t sessionId;
        int32_t data1;
        int32_t data2;
        int32_t data3;
        int32_t data4;
        int32_t data5;
        MSGPACK_DEFINE(packetId, sessionId,
        data1,
        data2,
        data3,
        data4,
        data5);
    };
} // End packets namespace


#endif //CELEGRAPH_PACKETS_H
