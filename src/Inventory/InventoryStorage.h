//
// Created by Mario on 12/05/2023.
//

#ifndef SRC_INVENTORYSTORAGE_H
#define SRC_INVENTORYSTORAGE_H

#include "Storage.h"

/**
 * @class InventoryStorage (a child class of Storage)
 *
 * The Inventory of the main character.
 * */
class InventoryStorage : public Storage
{
    public:
        InventoryStorage();
        InventoryStorage(const InventoryStorage &other);
        ~InventoryStorage() override ;

        /** In addition to Storage::add, this also equips the entity, if appropriate */
        bool add(const std::shared_ptr<GameEntity>& entity, unsigned int quantity = 1) override;

        /**
        * Puts an object that's already in the storage into one of the hands.
        * @param object name (id)
        * @return true unless the hand is holding something or the object is invalid
        */
        bool equip(std::shared_ptr<GameEntity> entity);

        /**
        * Removes an object from hands ( / attempts to )
        * if the player is holding two instances of one object, only removes one of them
        * @param object
        * @return true if object was in one of the hands & now was removed,
        */
        bool unequip(std::shared_ptr<GameEntity> entity);

        /** In addition to Storage::removeOne, this also un-equips if appropriate */
        bool removeOne(std::shared_ptr<GameEntity> entity) override;

        /** @returns string with contents of the storage, override because this adds info about hands */
        virtual std::string toString() const override;

        /** Removes any object(s) from hands. */
        void clearHands();

        /** Get objectInLeftHand, if it's a weapon, nullptr otherwise. */
        std::shared_ptr<GameEntity> getWeaponInLeftHand();

        /** Get objectInRightHand, if it's a weapon, nullptr otherwise. */
        std::shared_ptr<GameEntity> getWeaponInRightHand();

private:

        /**
         * What the main character has in their left/right hand.
         * Both are nullable.
         * These objects are objects from the objects map (see Storage.h)
         * */
        std::shared_ptr<GameEntity> objectInLeftHand;
        std::shared_ptr<GameEntity> objectInRightHand;

};

#endif //SRC_INVENTORYSTORAGE_H
