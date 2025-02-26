//
// Created by Mario on 12/05/2023.
//

#ifndef SRC_STORAGE_H
#define SRC_STORAGE_H

#include <unordered_map>
#include "../GameEntity/GameObject.h"

/**
 * @class Storage
 * A storage containing Entities, it can include multiple instances of one Entity.
 * Storage can contain objects of any of the classes in the GameEntities (including NPCs)
 * */
class Storage
{
    public:

        Storage();
        Storage(const Storage &other);
        virtual ~Storage();

        /**
        * Adds an entity instance
        * @param entity
        * @param quantity - the number of instances to add, 1 if unspecified
        */
        virtual bool add(const std::shared_ptr<GameEntity>& entity, unsigned int quantity = 1);

        /**
        * Removes an entity instance.
        * @param entity
        */
        virtual bool removeOne(std::shared_ptr<GameEntity> entity);

        /**
        * Returns the number of instances of given entity in this storage
        * @param entity
        * @return entity count
        */
        unsigned int getEntityCount(const std::shared_ptr<GameEntity>& entity) const;

        /** @return the objects of the storage in a string form */
        virtual std::string toString() const;

        /** Attempts to retrieve an entity in the story by its name. */
        std::shared_ptr<GameEntity> getEntityFromName(const std::string& name);

        // Basic getters and setters follow.

        unsigned int getSlotsUsed() const;

        unsigned int getSlotsTotal() const;

        void setSlotsTotal(unsigned int total);

    protected:

        /** storage capacity */
        unsigned int slotsTotal;

        unsigned int slotsUsed;

        /** map where key is the entity's unique name, and value is pair < pointer to the object, the object count in this storage > */
        std::unordered_map< std::string, std::pair<std::shared_ptr<GameEntity>, unsigned int > > entitiesInStorage;
};

#endif //SRC_STORAGE_H
