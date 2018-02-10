// File:     Player.cpp
// Created:  April 19, 2015
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any player in Oldentide.
//        :  Inherits from "Character" class.

#include "Player.h"
#include <netinet/in.h>

//------------------------------------------------------------------------------------------------//
//-------------------                     Class Constructors                   -------------------//
//------------------------------------------------------------------------------------------------//

Player::Player(
    sockaddr_in client,
    std::string account,
    int id,
    int session,
    skills_t skills,
    std::string firstname,
    std::string lastname,
    std::string guild,
    std::string race,
    std::string gender,
    std::string face,
    std::string skin,
    std::string zone,
    std::string profession,
    equipment_t equipment,
    stats_t stats,
    location_t location
)
: Character(
    firstname,
    lastname,
    guild,
    race,
    gender,
    face,
    skin,
    zone,
    profession,
    equipment,
    stats,
    location
)
{
    SetClient(client);
    SetAccount(account);
    SetId(id);
    SetSession(session);
    SetSkills(skills);
}

Player::~Player() {
    return;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Getter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

sockaddr_in Player::GetClient() {
    return client;
}

std::string Player::GetAccount() {
    return account;
}

int Player::GetId() {
    return id;
}

int Player::GetSession() {
    return session;
}

skills_t Player::GetSkills() {
    return skills;
}



//------------------------------------------------------------------------------------------------//
//-------------------                      Setter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

void Player::SetClient(sockaddr_in client) {
    this->client = client;
}

void Player::SetAccount(std::string account) {
    this->account = account;
}

void Player::SetId(int id) {
    this->id = id;
}

void Player::SetSession(int session) {
    this->session = session;
}

void Player::SetSkills(skills_t skills) {
    this->skills = skills;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Class Functions                     -------------------//
//------------------------------------------------------------------------------------------------//

