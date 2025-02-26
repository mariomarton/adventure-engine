//
// Created by Mario on 25/05/2023.
//

#include "GameObject.h"

using map_StringUnsigned_Tuple = std::map< std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> >;

GameObject::GameObject( const std::string& name,
                        unsigned int maxStage,
                        bool canBeTaken,
                        std::unique_ptr< map_StringUnsigned_Tuple > actionResponses,
                        std::unique_ptr<CombatStats> combatStats ) :
    GameEntity(name, maxStage, std::move(actionResponses), std::move(combatStats)),
    canBeTaken(canBeTaken)
    {}

GameObject::GameObject(const GameObject &other) :
    GameEntity(other),
    canBeTaken(other.canBeTaken)
    {}

GameObject::~GameObject()
{}

bool GameObject::canTakePartInCombat() const
{
    if(!combatStats)
        return false;

    // If usesRemaining is 0, the object (weapon) cannot be used.
    return combatStats->usesRemaining >= 1;
}

bool GameObject::canBeAddedToInventory() const
{
    return canBeTaken;
}
