// File:     Character.h
// Created:  April 19, 2015
// Author:   Joseph DeVictoria
// Purpose:  Child class representing any living object in Oldentide.

#ifndef OLDENTIDE_CHARACTER_H_
#define OLDENTIDE_CHARACTER_H_

#include <vector>
#include <string>
#include "Item.h"

typedef struct {
    std::string head;
    std::string chest;
    std::string arms;
    std::string hands;
    std::string legs;
    std::string feet;
    std::string cloak;
    std::string necklace;
    std::string ringone;
    std::string ringtwo;
    std::string righthand;
    std::string lefthand;
} equipment_t;


typedef struct {
    int level;
    int hp;
    int maxhp;
    int bp;
    int maxbp;
    int mp;
    int maxmp;
    int ep;
    int maxep;
    int strength;
    int constitution;
    int intelligence;
    int dexterity;
} stats_t;


typedef struct {
    float x;
    float y;
    float z;
    float pitch;
    float yaw;
} location_t;



class Character {
    private:

    std::string firstname;
    std::string lastname;
    std::string guild;
    std::string race;
    std::string gender;
    std::string face;
    std::string skin;
    std::string zone;
    std::string profession;
    equipment_t equipment;
    stats_t stats;
    location_t location;
    std::vector<Item *> inventory;

    public:

    // Constructors
    Character(
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

    ~Character();

    // Getter Functions
    std::string GetFirstname();
    std::string GetLastname();
    std::string GetGuild();
    std::string GetRace();
    std::string GetGender();
    std::string GetFace();
    std::string GetSkin();
    std::string GetZone();
    std::string GetProfession();
    equipment_t GetEquipment();
    stats_t GetStats();
    location_t GetLocation();
    // std::vector<Item *> GetInventory();

    // Setter Functions
    void SetFirstname(std::string);
    void SetLastname(std::string);
    void SetGuild(std::string);
    void SetRace(std::string);
    void SetGender(std::string);
    void SetFace(std::string);
    void SetSkin(std::string);
    void SetZone(std::string);
    void SetProfession(std::string);
    void SetEquipment(equipment_t);
    void SetStats(stats_t);
    void SetLocation(location_t);

    // Class Functions
    // TODO: Add inventory item
    // TODO: Remove inventory item
    // TODO: Get inventory item
};

#endif //OLDENTIDE_CHARACTER_H_

