// File:     Npc.h
// Created:  January 31, 2016
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any NPC in Oldentide.
//        :  Inherits from "Character" class.

#ifndef OLDENTIDE_NPC_H_
#define OLDENTIDE_NPC_H_

#include "Character.h"
#include <string>

class Npc : public Character {

    private:

    int id;

    public:

    // Constructors.
    Npc(
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
    );

    ~Npc();

    // Getter Functions.
    int GetId();

    // Setter Functions.
    void SetId(int id);

    // Class Functions.

};

#endif //OLDENTIDE_NPC_H_

