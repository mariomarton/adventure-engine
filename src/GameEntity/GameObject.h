//
// Created by Mario on 11/05/2023.
//

#ifndef SRC_GAMEOBJECT_H
#define SRC_GAMEOBJECT_H

#include "GameEntity.h"
#include <string>
#include <vector>

/**
 * @class GameObject (a child class of GameEntity)
 * An object the player can interact with.
 * */
class GameObject : public GameEntity
{
    public:

        GameObject( const std::string& name,
                    unsigned int maxStage,
                    bool canBeTaken,
                    std::unique_ptr<std::map<std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>>> actionResponses,
                    std::unique_ptr<CombatStats> combatStats
                    );

        GameObject(const GameObject &other);
        ~GameObject() override;

        /**
         * If true, the object can be stored in the InventoryStorage (e.g. food, weapon).
         * If false, it can't (e.g. door in a room).
         */
        const bool canBeTaken;

        /**
         * Overrides the virtual method.
         * @return true if this object is a weapon
         * */
        [[nodiscard]] bool canTakePartInCombat() const override;

        /**
         * Overrides the virtual method.
         * @return true if this object is a move-able
         * */
        [[nodiscard]] bool canBeAddedToInventory() const override;
};

#endif //SRC_GAMEOBJECT_H
