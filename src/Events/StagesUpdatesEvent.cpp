//
// Created by Mario on 03/06/2023.
//

#include "StagesUpdatesEvent.h"

StagesUpdatesEvent::StagesUpdatesEvent(int id)
        : id(id), stageChangesMap(std::make_unique<std::map<std::string, unsigned int>>())
{}


StagesUpdatesEvent::StagesUpdatesEvent(const StagesUpdatesEvent& other)
        : id(other.id), stageChangesMap(nullptr)
{
    if (other.stageChangesMap)
        stageChangesMap = std::make_unique<std::map<std::string, unsigned int>>(*other.stageChangesMap);
}


StagesUpdatesEvent::~StagesUpdatesEvent()
{}

std::map<std::string, unsigned int>& StagesUpdatesEvent::getChangesMap()
{
    //the unique ptr will not be null, see the constructor
    return *stageChangesMap;
}


