// File:     Player.h
// Created:  April 19, 2015
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any player in Oldentide.
//        :  Inherits from "Character" class.

#ifndef OLDENTIDE_PLAYER_H_
#define OLDENTIDE_PLAYER_H_

#include "Character.h"
#include <vector>
#include <string>
#include <netinet/in.h>


typedef struct {
    int axe;
    int dagger;
    int unarmed;
    int hammer;
    int polearm;
    int spear;
    int staff;
    int sword;
    int archery;
    int crossbow;
    int sling;
    int thrown;
    int armor;
    int dualweapon;
    int shield;
    int bardic;
    int conjuring;
    int druidic;
    int illusion;
    int necromancy;
    int sorcery;
    int shamanic;
    int spellcraft;
    int summoning;
    int focus;
    int armorsmithing;
    int tailoring;
    int fletching;
    int weaponsmithing;
    int alchemy;
    int lapidary;
    int calligraphy;
    int enchanting;
    int herbalism;
    int hunting;
    int mining;
    int bargaining;
    int camping;
    int firstaid;
    int lore;
    int picklocks;
    int scouting;
    int search;
    int stealth;
    int traps;
    int aeolandis;
    int hieroform;
    int highgundis;
    int oldpraxic;
    int praxic;
    int runic;
} skills_t;


class Player : public Character {
    private:

    int id;
    int session;
    sockaddr_in client;
    std::string account;
    skills_t skills;

    public:

    // Constructors.
    Player(
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
    );

    ~Player();

    // Getter Functions.

    sockaddr_in GetClient();
    std::string GetAccount();
    int GetId();
    int GetSession();
    skills_t GetSkills();

    // Setter Functions.

    void SetClient(sockaddr_in client);
    void SetAccount(std::string account);
    void SetId(int id);
    void SetSession(int session);
    void SetSkills(skills_t);

    // Class Functions.
};

#endif //OLDENTIDE_PLAYER_H_
