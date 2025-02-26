//
// Created by Mario on 03/06/2023.
//

#ifndef SRC_STAGESUPDATESEVENT_H
#define SRC_STAGESUPDATESEVENT_H

#include "../GameEntity/GameEntity.h"
#include "../GameEntity/GameObject.h"
#include <memory>

/**
 * @class StagesUpdatesEvent
 * A series of changes to Game Entities that a player's action can cause
 * */
class StagesUpdatesEvent
{
    public:
        StagesUpdatesEvent(int id);
        StagesUpdatesEvent(const StagesUpdatesEvent& other);
        ~StagesUpdatesEvent();

        const int id;

        std::map<std::string, unsigned int>& getChangesMap();

    private:
        /** Maps entity name to its new stage */
        std::unique_ptr<std::map<std::string, unsigned int>> stageChangesMap;
};

#endif // SRC_STAGESUPDATESEVENT_H