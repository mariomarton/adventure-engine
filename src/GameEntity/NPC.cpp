//
// Created by Mario on 25/05/2023.
//

#include "NPC.h"

using map_StringUnsigned_Tuple = std::map< std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> >;

NPC::NPC(    const std::string& name,
             unsigned int maxStage,
             std::unique_ptr< map_StringUnsigned_Tuple > actionResponses,
             unsigned int health,
             std::unique_ptr<CombatStats> combatStats
) :
        GameEntity(name, maxStage, std::move(actionResponses), std::move(combatStats)),
        health(health)
{}

NPC::NPC(const NPC& other) :
    GameEntity(other),
    health(other.health)
{}

NPC::~NPC()
{}

unsigned int NPC::getHealthAfterDecrease(unsigned int decrease)
{
    if(decrease > health)
    {
        health = 0;
        return 0;
    }

    health -= decrease;
    return health;
}

bool NPC::canTakePartInCombat() const
{
    /*
     * If dead or not fightable, return false
     * Else, return true, even if combatStats->usesRemaining is 0.
     * (The Main Character can fight the NPC even if it can't defend itself.)
     */
    return !(health == 0 || !combatStats);
}
