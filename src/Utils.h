// Header for miscellaneous functions.

#ifndef CELEGRAPH_UTILS_H
#define CELEGRAPH_UTILS_H

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <msgpack.hpp>

namespace utils{
    std::vector<std::string> Tokenfy(std::string s, char delim);
    bool CheckPasswordLength(std::string);
    bool CheckAccountNameLength(std::string);
    bool SanitizeAccountName(std::string);
    bool SanitizeAlphanumeric(std::string);
    bool SanitizeHexString(std::string);
    void PrintStringHex(std::string *);
    void PrependPacketHeader(std::string *, uint8_t);
    uint8_t GetPacketTypeFromPacket(char *);
    uint16_t GetMsgpckSizeFromPacket(char *);
    std::string GetMsgpckDataFromPacket(char *packetBuffer);
    std::string GetIpAndPortFromSocket(sockaddr_in *socket);

    // Convenience wrapper functions
    msgpack::object_handle ReceiveDataFrom(int sockfd,  uint8_t *packetTypeOut, sockaddr_in *sourceOut);
    int SendDataTo(int sockfd, std::stringstream *dataIn, uint8_t packetTypeIn, sockaddr_in *destIn);

    // More specific functions for the server's packetQueue
    void ReceivePacketFrom(int sockfd, char *packetBufferOut, sockaddr_in *sourceOut);
    msgpack::object_handle GetDataFromPacket(char *packetBufferIn, uint8_t *packetTypeOut);
    void SendErrorTo(int sockfd, std::string errorMsg, sockaddr_in *destIn);

};


#endif // CELEGRAPH_UTILS_H
