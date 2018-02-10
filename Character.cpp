// File:     Character.cpp
// Created:  April 19, 2015
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any living object in Oldentide.

#include "Character.h"
#include "Utils.h"






//------------------------------------------------------------------------------------------------//
//-------------------                     Class Constructors                   -------------------//
//------------------------------------------------------------------------------------------------//

Character::Character(
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
) {
    SetFirstname(firstname);
    SetLastname(lastname);
    SetGuild(guild);
    SetRace(race);
    SetGender(gender);
    SetFace(face);
    SetSkin(skin);
    SetZone(zone);
    SetProfession(profession);
    SetEquipment(equipment);
    SetStats(stats);
    SetLocation(location);
}

Character::~Character() {
    return;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Getter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

std::string Character::GetFirstname() {
    return firstname;
}

std::string Character::GetLastname() {
    return lastname;
}

std::string Character::GetGuild() {
    return guild;
}

std::string Character::GetRace() {
    return race;
}

std::string Character::GetGender() {
    return gender;
}

std::string Character::GetFace() {
    return face;
}

std::string Character::GetSkin() {
    return skin;
}

std::string Character::GetZone() {
    return zone;
}

std::string Character::GetProfession() {
    return profession;
}

stats_t Character::GetStats() {
    return stats;
}


equipment_t Character::GetEquipment() {
    return equipment;
}

location_t Character::GetLocation() {
    return location;
}

// std::vector<Item *> Character::GetInventory() {
//     return inventory;
// }

//------------------------------------------------------------------------------------------------//
//-------------------                      Setter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

void Character::SetFirstname(std::string firstname) {
    this->firstname = firstname;
}

void Character::SetLastname(std::string lastname) {
    this->lastname = lastname;
}

void Character::SetGuild(std::string guild) {
    this->guild = guild;
}

void Character::SetRace(std::string race) {
    this->race = race;
}

void Character::SetGender(std::string gender) {
    this->gender = gender;
}

void Character::SetFace(std::string face) {
    this->face = face;
}

void Character::SetSkin(std::string skin) {
    this->skin = skin;
}

void Character::SetZone(std::string zone) {
    this->zone = zone;
}

void Character::SetProfession(std::string profession) {
    this->profession = profession;
}

void Character::SetStats(stats_t stats) {
    this->stats = stats;
}

void Character::SetEquipment(equipment_t equipment) {
    this->equipment = equipment;
}

void Character::SetLocation(location_t location) {
    this->location = location;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Class Functions                     -------------------//
//------------------------------------------------------------------------------------------------//

