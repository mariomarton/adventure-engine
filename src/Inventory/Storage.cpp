//
// Created by Mario on 15/05/2023.
//

#include "Storage.h"

Storage::Storage() :
        slotsTotal(0),
        slotsUsed(0)
{}

Storage::Storage(const Storage& other) :
        slotsTotal(other.slotsTotal),
        slotsUsed(other.slotsUsed)
{
    //create a semi-deep copy of other.entitiesInStorage:
    for (const auto& pair : other.entitiesInStorage)
    {
        const std::string& key = pair.first;
        std::pair<std::shared_ptr<GameEntity>, unsigned int> value = pair.second;
        entitiesInStorage[key] = value;
    }
}

Storage::~Storage()
{}

unsigned int Storage::getEntityCount(const std::shared_ptr<GameEntity>& entity) const
{
    if(!entity)
        return 0;

    auto it = entitiesInStorage.find(entity->name);
    if (it != entitiesInStorage.end())
        return it->second.second;

    return 0;
}

bool Storage::add(const std::shared_ptr<GameEntity>& entity, unsigned int quantity)
{
    //default implementation for Base class

    if(!entity)
        return false;

    if (slotsUsed + quantity > slotsTotal)
        return false;

    if (entitiesInStorage.find(entity->name) == entitiesInStorage.end())
    {
        entitiesInStorage[entity->name].first = entity;
        entitiesInStorage[entity->name].second = quantity;
    }
    else
        entitiesInStorage[entity->name].second += quantity;

    slotsUsed += quantity;
    return true;
}

bool Storage::removeOne(std::shared_ptr<GameEntity> entity)
{
    if(!entity)
        return false;

    auto count = getEntityCount(entity);

    if (count > 0)
    {
        --entitiesInStorage[entity->name].second;
        --slotsUsed;
        if (entitiesInStorage[entity->name].second == 0) {
            entitiesInStorage.erase(entity->name);
        }
        return true;
    }
    return false;
}

unsigned int Storage::getSlotsUsed() const
{
    return slotsUsed;
}

unsigned int Storage::getSlotsTotal() const
{
    return slotsTotal;
}

void Storage::setSlotsTotal(unsigned int total)
{
    slotsTotal = total;
}

std::shared_ptr<GameEntity> Storage::getEntityFromName(const std::string& name)
{
    auto it = entitiesInStorage.find(name);

    if (it != entitiesInStorage.end())
    {
        // Access the shared_ptr<GameEntity> from the pair
        std::shared_ptr<GameEntity> objectPtr = it->second.first;

        // Return the shared_ptr<GameEntity>
        return objectPtr;
    }

    // If the entry does not exist, return nullptr
    return nullptr;
}

std::string Storage::toString() const
{
    std::string storageStr;

    for (const auto& pair : entitiesInStorage)
    {
        const std::string& name = pair.first;
        std::shared_ptr<GameEntity> objectPtr = pair.second.first;
        unsigned int quantity = pair.second.second;

        // Capitalize the first letter of the name
        std::string capitalized = name;
        if (!capitalized.empty())
        {
            capitalized[0] = std::toupper(capitalized[0]);
        }

        // Append " - " followed by the capitalized name to the result string
        storageStr += " - " + capitalized + " (" + std::to_string(quantity) + ")\n";
    }

    return storageStr;
}




