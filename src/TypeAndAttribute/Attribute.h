//
// Created by Mario on 15/05/2023.
//

#ifndef SRC_ATTRIBUTE_H
#define SRC_ATTRIBUTE_H

#include <string>

static const int DEFAULT_NEW_ATTRIBUTE_VALUE = 100; // 100 percent.

/**
 * @class Attribute
 * An attribute that the main character and NPCs can have, defined by config files, e.g. could be health, power.
 *
 * Attribute value depends on the player's choice of MainCharacterType and on player's choices regarding Attributes.
 * */
class Attribute
{
    public:
        Attribute(std::string name, unsigned int value = DEFAULT_NEW_ATTRIBUTE_VALUE);
        Attribute(const Attribute &other);
        ~Attribute();

        /**
        * Increases the value of Attribute
        * @param number of units to be added to this Attribute's value
        * Cannot fail as there is no upper limit for value
        * */
        void increaseValueBy(unsigned int increase);

        /**
        * Decreases the value of Attribute
        * @param number of units to be removed from this Attribute's value
        * @return true, unless the updated value would be negative
        * */
        bool decreaseValueBy(unsigned int decrease);

        /** The name of the attribute, e.g. Health. */
        const std::string name;

        // Getters and setters follow.

        void setValue(unsigned int n);

        [[nodiscard]] unsigned int getValue() const;

    private:

        unsigned int value; // 100 at the start of the game ( percentage, but can exceed 100 )

};

#endif //SRC_ATTRIBUTE_H
