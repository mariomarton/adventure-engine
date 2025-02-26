//
// Created by Mario on 12/05/2023.
//

#ifndef SRC_MAINCHARACTERTYPE_H
#define SRC_MAINCHARACTERTYPE_H

#include <string>
#include <vector>
#include <map>
#include "Attribute.h"

/**
 * @class MainCharacterType
 * MainCharacterType is the type of the player's character, chosen by the player at the start of a game.
 *
 * The chosen type further changes Main Character's attributes.
 * */
class MainCharacterType
{
    public:
        explicit MainCharacterType(const std::string &name);
        MainCharacterType(const MainCharacterType &other);
        ~MainCharacterType();

        /** @return multiplier indicating how this Type affects a specific Attribute
         *  e.g. if this type decreases the value of an attribute by 5 units, returns -5
         * */
        std::vector<unsigned int>& getAttributeIncreases();

        const std::string name;

    private:
        std::vector<unsigned int> attributeIncreases;
};

#endif //SRC_MAINCHARACTERTYPE_H
