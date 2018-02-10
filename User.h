#ifndef CELEGRAPH_USER_H
#define CELEGRAPH_USER_H

#include <vector>
#include <string>
#include <netinet/in.h>


class User {
    private:

    int id;
    int session;
    sockaddr_in client;
    std::string account;
    std::string firstname;
    std::string lastname;

    public:

    // Constructors.
    User(
        sockaddr_in client,
        std::string account,
        int id,
        int session,
        std::string firstname,
        std::string lastname
    );

    ~User();

    // Getter Functions.
    sockaddr_in GetClient();
    std::string GetAccount();
    int GetId();
    int GetSession();
    std::string GetFirstname();
    std::string GetLastname();

    // Setter Functions.
    void SetClient(sockaddr_in client);
    void SetAccount(std::string account);
    void SetId(int id);
    void SetSession(int session);
    void SetFirstname(std::string);
    void SetLastname(std::string);
};

#endif //CELEGRAPH_USER_H
