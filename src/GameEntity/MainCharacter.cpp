//
// Created by Mario on 25/05/2023.
//

#include "MainCharacter.h"
#include <cmath>
#include <algorithm>
#include <utility>

using map_StringUnsigned_Tuple = std::map< std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> >;

MainCharacter::MainCharacter(   const std::string& name,
                                unsigned int maxStage,
                                InventoryStorage &inventoryStorage,
                                std::unique_ptr< map_StringUnsigned_Tuple > actionResponses ) :
    GameEntity(name, maxStage, std::move(actionResponses)),
    inventoryStorage(inventoryStorage),
    weaponInUse(WEAPON_NONE)
{
    attributes = std::vector< std::unique_ptr<Attribute> >();
}


MainCharacter::MainCharacter(const MainCharacter& other) :
        GameEntity(other),
        type(other.type ? std::make_unique<MainCharacterType>(*other.type) : nullptr),
        inventoryStorage(other.inventoryStorage),
        weaponInUse(other.weaponInUse)
{
    attributes.reserve(other.attributes.size());

    for (const auto& attribute : other.attributes) {
        attributes.emplace_back(std::make_unique<Attribute>(*attribute));
    }
}

MainCharacter::~MainCharacter() = default;


bool MainCharacter::meetsRequirements(std::vector<unsigned int>& attributeValues) {
    for( unsigned int i = 0; i < attributes.size(); i++ )
    {
        if(attributeValues[i] > attributes[i]->getValue())
            return false; // One of the attributes not met.
    }
    return true;
}

unsigned int MainCharacter::getHealthAfterDecrease(unsigned int decrease)
{
    unsigned int health = attributes[0]->getValue();

    if(decrease > health)
    {
        //the character's health would be less than 0, so we set it to 0
        attributes[0]->decreaseValueBy(health);
        return 0;
    }

    attributes[0]->decreaseValueBy(decrease);

    health -= decrease;
    return health;
}

int MainCharacter::prepareForCombatAndGetPower()
{
    std::shared_ptr<GameEntity> weaponL = inventoryStorage.getWeaponInLeftHand();
    std::shared_ptr<GameEntity> weaponR = inventoryStorage.getWeaponInRightHand();

    // Get the total power of the possible weapon in each hand

    int powerWeaponL = 0;
    int powerWeaponR = 0;

    // And also get the power with no weapon

    double handsPowerTemp = COMBAT_DEFAULT_DAMAGE * attributes[1]->getValue() * COMBAT_DEFAULT_SPEED;
    handsPowerTemp /= 10000;
    int powerHands = std::lrint(handsPowerTemp);


    if(weaponL)
        powerWeaponL = weaponL->prepareForCombatAndGetPower();

    if(weaponR)
        powerWeaponR = weaponR->prepareForCombatAndGetPower();

    // 3. Compare the three powers, choose the most effective option, as set the weaponInUse

    int thePower = std::max({powerWeaponL, powerWeaponR, powerHands});

    if( (powerWeaponL > powerHands) || (powerWeaponR > powerHands) )
    {
        //using a weapon, let's find out which one
        if(powerWeaponL >= powerWeaponR)
            weaponInUse = WEAPON_LEFT;
        else
            weaponInUse = WEAPON_RIGHT;
    }
    else weaponInUse = WEAPON_NONE;

    return thePower;
}

std::shared_ptr<GameEntity> MainCharacter::getWeaponInUse()
{
    if(weaponInUse == WEAPON_LEFT)
        return inventoryStorage.getWeaponInLeftHand();

    else if(weaponInUse == WEAPON_RIGHT)
        return inventoryStorage.getWeaponInRightHand();

    else return nullptr;
}

// Basic getters and setters follow.

void MainCharacter::setType(std::shared_ptr<MainCharacterType> mainCharacterType)
{
    type = std::move(mainCharacterType);
}


std::vector<std::unique_ptr<Attribute>>& MainCharacter::getAttributes()
{
    return attributes;
}

std::shared_ptr<MainCharacterType> MainCharacter::getType()
{
    return type;
}

