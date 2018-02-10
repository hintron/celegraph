// File:     Item.h
// Created:  October 10, 2017
// Purpose:  Represents an item that can be possessed by NPCs or players

#ifndef OLDENTIDE_ITEM_H_
#define OLDENTIDE_ITEM_H_

#include <string>

class Item {

    private:

    int id;

    // TODO: Is there any use for putting the owner inside the item?
    // How to do this with c++ classes?
    // // Either an npc or a player can be possessing an item
    // Character* owner;

    public:
    // TODO: Make these private, create getters
    std::string name;
    std::string location;
    std::string weight;
    bool equip;
    bool use;
    int x;
    int y;

    // Constructors.
    Item(
        int id,
        std::string name,
        std::string location,
        std::string weight,
        bool equip,
        bool use,
        int x,
        int y
    );

    ~Item();

    // Getter Functions.
    std::string GetName();
    // Character * GetOnwer();

    // Setter Functions.
    // void SetOwner(Character *owner);

    // Class Functions.

};

#endif //OLDENTIDE_ITEM_H_

