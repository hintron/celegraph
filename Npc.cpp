// File:     Npc.cpp
// Created:  January 31, 2016
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any NPC in Oldentide.
//        :  Inherits from "Character" class.

#include "Npc.h"
#include "Utils.h"

//------------------------------------------------------------------------------------------------//
//-------------------                     Class Constructors                   -------------------//
//------------------------------------------------------------------------------------------------//

Npc::Npc(
    int id,
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
:Character(
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
) {
    SetId(id);
}

Npc::~Npc() {
    return;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Getter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

int Npc::GetId() {
    return id;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Setter Functions                    -------------------//
//------------------------------------------------------------------------------------------------//

void Npc::SetId(int id) {
    this->id = id;
}

//------------------------------------------------------------------------------------------------//
//-------------------                      Class Functions                     -------------------//
//------------------------------------------------------------------------------------------------//
