//
// Created by Mario on 25/05/2023.
//

#include "GameEntity.h"
#include <stdexcept>
#include <utility>
#include <cmath>

using map_StringUnsigned_Tuple = std::map< std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> >;

GameEntity::GameEntity( std::string name,
                        unsigned int maxStage,
                        std::unique_ptr< map_StringUnsigned_Tuple > actionResponses,
                        std::unique_ptr<CombatStats> combatStats) :
          name(std::move(name)),
          stage(0),
          maxStage(maxStage),
          actionResponses( std::move(actionResponses) ),
          combatStats( std::move(combatStats) ) //can be null, that's ok
{}


GameEntity::GameEntity(const GameEntity &other) :
    name(other.name),
    stage(other.stage),
    maxStage(other.maxStage),
    actionResponses(std::make_unique< map_StringUnsigned_Tuple >(*other.actionResponses))
{
    if(other.canTakePartInCombat())
        combatStats = std::make_unique< CombatStats >(*other.combatStats);
    else
        combatStats = nullptr;
}

GameEntity::~GameEntity() = default;

std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>& GameEntity::getActionResponse(const std::string& action) const
{
    // Create a key pair with the action and the current stage
    std::pair<std::string, unsigned int> actionPlusStageKey(action, stage);

    // Check if the key exists in the actionResponses map
    auto it = actionResponses->find(actionPlusStageKey);
    if (it != actionResponses->end()) {
        return it->second;
    }

    throw std::invalid_argument("No response found for the specified action and the current stage of this GameEntity.");
}

bool GameEntity::setStageTo(unsigned int newStage)
{
    if(newStage > maxStage)
        return false;

    stage = newStage;
    return true;
}



int GameEntity::prepareForCombatAndGetPower()
{
    //this default implementation only calculates and returns the power

    if(!combatStats)
        return 0;

    double res = COMBAT_DEFAULT_DAMAGE * combatStats->hitPower * combatStats->hitSpeed;
    res /= 10000;
    return std::lrint(res);
}

bool GameEntity::decreaseUsesRemaining()
{
    if(!canTakePartInCombat())
        return false;
    else
        combatStats->usesRemaining--;

    return true;
}

bool GameEntity::canBeAddedToInventory() const
{
    return false;
}

bool GameEntity::canTakePartInCombat() const
{
    // this default implementation is only relevant to MainCharacter, other classes override it
    return false;
}

unsigned int GameEntity::getHealthAfterDecrease([[maybe_unused]] unsigned int decrease)
{
    throw std::invalid_argument("Not applicable to this entity.");
}