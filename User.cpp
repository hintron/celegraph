#include "User.h"
#include <netinet/in.h>

User::User(
    sockaddr_in client,
    std::string account,
    int id,
    int session,
    std::string firstname,
    std::string lastname
)
{
    // SetClient(client);
    // SetAccount(account);
    // SetId(id);
    // SetSession(session);
    // SetSkills(skills);
}

User::~User() {
    return;
}

/////////////////
// GETTERS
/////////////////

sockaddr_in User::GetClient() {
    return client;
}

std::string User::GetAccount() {
    return account;
}

int User::GetId() {
    return id;
}

int User::GetSession() {
    return session;
}

std::string User::GetFirstname() {
    return firstname;
}

std::string User::GetLastname() {
    return lastname;
}

/////////////////
// SETTERS
/////////////////

void User::SetClient(sockaddr_in client) {
    this->client = client;
}

void User::SetAccount(std::string account) {
    this->account = account;
}

void User::SetId(int id) {
    this->id = id;
}

void User::SetSession(int session) {
    this->session = session;
}

void User::SetFirstname(std::string firstname) {
    this->firstname = firstname;
}

void User::SetLastname(std::string lastname) {
    this->lastname = lastname;
}

