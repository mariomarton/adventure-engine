//
// Created by Mario on 11/05/2023.
//

#ifndef SRC_GAMEENTITY_H
#define SRC_GAMEENTITY_H

#include <string>
#include <unordered_set>
#include <map>
#include <vector>
#include <memory>
#include "CombatStats.h"

// Game Entities manage a lot of the Combat mechanics, that's why we have these constants here:

// Combat damage for one hit by hand (by default HP is 100, so 5 is 5% of damage)
[[maybe_unused]] static unsigned int COMBAT_DEFAULT_DAMAGE = 5; // so in total, default fight will cause minus 50 HP

// Combat speed for hand combat - equivalent of hitSpeed for hands
[[maybe_unused]] static unsigned int COMBAT_DEFAULT_SPEED = 100;

/**
 * @class GameEntity (Parent class of MainCharacter, NPC & GameObject)
 * GameEntity represents an entity that the player can interact with in the game.
 *
 * The player can call commands featuring this entity
 * e.g. "Fight the ENEMY", "Push that DOOR", "Add APPLE" (GameEntity is capitalised)
 * */
class GameEntity
{
    public:

        GameEntity( std::string name,
                    unsigned int maxStage,
                    std::unique_ptr<std::map<std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>>> actionResponses,
                    std::unique_ptr<CombatStats> combatStats = nullptr
                    );

        GameEntity(const GameEntity &other);
        virtual ~GameEntity();

        const std::string name;

        /**
         * Get response to player's interaction with the GameEntity
         * @param action (for example "PUSH" or "BURN")
         * @return the response to be printed and the event to take place
         * @throw std::invalid_argument if no response found
         */
        [[nodiscard]] std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>& getActionResponse(const std::string& action) const;

        /**
        * Change the stage of the GameEntity.
        * @param newStage
        * @return true if stage was set to newStage
        */
        bool setStageTo(unsigned int newStage);

        /**
        * Determine whether the entity can take part in combat or not.
        * The returned boolean can change in time depending on CombatStats (if no uses remain).
        */
        [[nodiscard]] virtual bool canTakePartInCombat() const;

        /**
        * Determine whether the entity can be added to Inventory (this doesn't check the inventory however).
        */
        virtual bool canBeAddedToInventory() const;

        /**
        * Calculate the power of the Entity.
        * If appropriate, it can also prepare for combat,
        * meaning it sets the weapon to be used in combat.
        * @return the power, 0 if not usable in combat
        */
        virtual int prepareForCombatAndGetPower();

        /**
        * Attempts to decrease entity's health by the 'decrease' value
        * @returns the new health value
        * @throws std::invalid_argument not applicable to this entity
        */
        virtual unsigned int getHealthAfterDecrease(unsigned int decrease);

        /** Attempts to decrease entity's CombatStats.usesRemaining. */
        bool decreaseUsesRemaining();

protected:
        /**
         * Each entity can have multiple stages, with different responses to actions in different stages.
         *
         * e.g. if GameEntity is a Door then stages could be:
         *      stage 0 -> The door is far, we don't know much about it.
         *      stage 1 -> We're closer to the door and can see details.
         *      stage 2 -> The door is open and we can walk through it.
         */

        /** Current stage. */
        unsigned int stage;

        /**
         *  Highest possible stage.
         *  Stages start at 0.
         * */
        const unsigned int maxStage;

        /**
         * Map for storing all responses to commands featuring this entity.
         *
         * Key   - pair < Command String, Stage >
         *       - e.g. pair< "ATTACK", 0 >
         *
         * Value - tuple < Main String, Alternative String, Attribute Requirements Vector, StagesUpdatesID, RoomChangeID >
         *       - where:
         *          - Main String is the string to be shown if Attribute Requirements are met.
         *          - Alternative String will be shown if they are not met.
         *          - Attribute Requirements Vector includes the minimum required Value for each each attribute.
         *          - StagesUpdatesID is the ID of the StagesUpdatesEvent that the action will cause.
         *          - RoomChangeID is the ID of the new room, or if it's negative it means the game ends.
         */
        std::unique_ptr<std::map<std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>>> actionResponses;

        /**
         * Combat Statistics, they can change during the game.
         * This can be null if entity not suitable for combat.
         */
        std::unique_ptr<CombatStats> combatStats;
};

#endif //SRC_GAMEENTITY_H
