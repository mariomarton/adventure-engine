//
// Created by Mario on 12/05/2023.
//

#ifndef SRC_MAINCHARACTER_H
#define SRC_MAINCHARACTER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "GameEntity.h"
#include "../Inventory/InventoryStorage.h"
#include "../TypeAndAttribute/MainCharacterType.h"

// The main character can have one object in each hand.
const static int WEAPON_NONE  = 100; // Constant meaning MC will use no weapon in Combat.
const static int WEAPON_LEFT  = 200; // Weapon in the left hand will be used.
const static int WEAPON_RIGHT = 300; // Weapon in the right hand will be used.

/**
 * @class MainCharacter (a child class of GameEntity)
 * The one and only main playable character of the Game.
 * */
class MainCharacter : public GameEntity
{
    public:

        MainCharacter(  const std::string& name,
                        unsigned int maxStage,
                        InventoryStorage &inventoryStorage,
                        std::unique_ptr<std::map<std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int>>> actionResponses
                        );
        MainCharacter(const MainCharacter &other);
        ~MainCharacter() override;

        /** Determines if the MainCharacter meets the requirements to display the main string for an action */
        bool meetsRequirements(std::vector<unsigned int>& attributeValues);

        /**
         * Decreases the Main Character's health by the 'decrease' value
         * @returns the new health value */
        unsigned int getHealthAfterDecrease(unsigned int decrease) override;

        /**
        * Calculate the power of the MainCharacter.
        * Overrides the virtual method.
        * Compares the power of the most powerful weapon in hands to the power with no weapon and returns the max value.
        * Prepares for combat by setting the weapon that will be used.
        * @return the power, 0 if not usable in combat
        */
        int prepareForCombatAndGetPower() override;

        /**
         * @returns a pointer to the currently most powerful weapon equipped.
         * or nullptr if bare hands are to be used for combat
         * */
        std::shared_ptr<GameEntity> getWeaponInUse();

        // Basic getters/setters follow.

        void setType(std::shared_ptr<MainCharacterType> mainCharacterType);

        std::vector<std::unique_ptr<Attribute>>& getAttributes();

        std::shared_ptr<MainCharacterType> getType();

    private:

        /**
         *  The type of the character. E.g. magician, or intellectual.
         *  This is chosen by the player at the start of the game.
         *  */
        std::shared_ptr< MainCharacterType > type;

        /** The only inventoryStorage instance in a game */
        InventoryStorage &inventoryStorage;

        /** Attributes */
        std::vector< std::unique_ptr<Attribute> > attributes;

        /** States which weapon (in left/right hand, or no weapon). Uses constants like WEAPON_NONE */
        int weaponInUse;
};

#endif //SRC_MAINCHARACTER_H
