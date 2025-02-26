//
// Created by Mario on 15/05/2023.
//

#include "InventoryStorage.h"

InventoryStorage::InventoryStorage() :
    Storage(),
    objectInLeftHand(nullptr),
    objectInRightHand(nullptr)
{}

InventoryStorage::InventoryStorage(const InventoryStorage& other) :
    Storage(other),
    objectInLeftHand(other.objectInLeftHand),
    objectInRightHand(other.objectInRightHand)
{}

InventoryStorage::~InventoryStorage() = default;

bool InventoryStorage::add(const std::shared_ptr<GameEntity> &entity, unsigned int quantity) {
    if(! Storage::add(entity, quantity) )
        return false;

    //also equip if possible
    equip(entity);
    return true;
}

bool InventoryStorage::equip(std::shared_ptr<GameEntity> entity)
{
    if(entity == nullptr)
        return false;

    unsigned int count = getEntityCount(entity);

    if(count == 0)
        return false;

    if (objectInLeftHand != nullptr && objectInRightHand != nullptr )
        return false;

    if (objectInLeftHand == nullptr)
        objectInLeftHand = entity;
    else
        objectInRightHand = entity;

    return true;
}

bool InventoryStorage::unequip(std::shared_ptr<GameEntity> entity)
{
    if(objectInLeftHand){
        if (entity->name == objectInLeftHand->name) {
            objectInLeftHand = nullptr;
            return true;
        }
    }

    if(objectInRightHand){
        if (entity->name == objectInRightHand->name) {
            objectInRightHand = nullptr;
            return true;
        }
    }

    return false;
}

bool InventoryStorage::removeOne(std::shared_ptr<GameEntity> entity)
{
    unsigned int count = getEntityCount(entity);

    if(count == 1){
        // attempt to remove from hands first
        unequip(entity);
    }

    return Storage::removeOne(entity);
}

std::shared_ptr<GameEntity> InventoryStorage::getWeaponInLeftHand()
{
    if(!objectInLeftHand)
        return nullptr;

    if(objectInLeftHand->canTakePartInCombat())
        return objectInLeftHand;

    return nullptr;
}

std::shared_ptr<GameEntity> InventoryStorage::getWeaponInRightHand()
{
    if(!objectInRightHand)
        return nullptr;

    if(objectInRightHand->canTakePartInCombat())
        return objectInRightHand;

    return nullptr;
}

std::string InventoryStorage::toString() const
{
    // Get the Base-class string
    std::string inventoryStr = Storage::toString();

    // Append hand information
    std::string leftObjectName = "-";
    std::string rightObjectName = "-";

    if(objectInLeftHand)
        leftObjectName = objectInLeftHand->name;

    if(objectInRightHand)
        rightObjectName = objectInRightHand->name;

    inventoryStr += "\n[L] : " + leftObjectName + "\n[R] : " + rightObjectName + "\n";

    return inventoryStr;
}

void InventoryStorage::clearHands() {
    objectInLeftHand = nullptr;
    objectInRightHand = nullptr;
}

