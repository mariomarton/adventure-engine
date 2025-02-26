//
// Created by Mario on 12/05/2023.
//

#ifndef SRC_ROOM_H
#define SRC_ROOM_H

#include <string>
#include <map>
#include "GameEntity/GameEntity.h"
#include "Inventory/Storage.h"
#include "GameEntity/NPC.h"

/**
 * @class Room
 * A room in the game, it manages its GameEntities.
 * */
class Room
{
    public:
        Room();
        ~Room();

        // Getters and setters

        void setDescription(const std::string& str);
    
        [[nodiscard]] const std::string& getDescription() const;

        Storage& getRoomStorage();

private:
        std::string description;

        Storage roomStorage; // stores both objects and NPCs
};


#endif //SRC_ROOM_H
