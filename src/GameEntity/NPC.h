//
// Created by Mario on 11/05/2023.
//

#ifndef SRC_NPC_H
#define SRC_NPC_H

#include "GameEntity.h"
#include "../TypeAndAttribute/Attribute.h"
#include <string>
#include <vector>

static const unsigned int DEFAULT_NPC_HEALTH = 100; // full health of MC is 100, this is for NPC, used in configReader

/**
 * @class NPC (a child class of GameEntity)
 * Non-playable character - represents all alive entities in game except the Main Character.
 * */
class NPC : public GameEntity
{
    public:

        NPC(    const std::string& name,
                unsigned int maxStage,
                std::unique_ptr<std::map<std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>>> actionResponses,
                unsigned int health,
                std::unique_ptr<CombatStats> combatStats = nullptr
                );

        NPC(const NPC &other);
        ~NPC() override;

        /** Overrode virtual canTakePartInCombat() method. */
        [[nodiscard]] bool canTakePartInCombat() const override;

        /**
         * Decreases the health by the 'decrease' value
         * @returns the new health value
         * */
        unsigned int getHealthAfterDecrease(unsigned int decrease) override;

    private:

        unsigned int health;

};

#endif //SRC_NPC_H
