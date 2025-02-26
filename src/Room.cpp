//
// Created by Mario on 26/05/2023.
//

#include "Room.h"
#include <limits>

#include <utility>

Room::Room()
{
    roomStorage.setSlotsTotal(std::numeric_limits<unsigned int>::max());
}

Room::~Room()
{}

Storage& Room::getRoomStorage()
{
    return roomStorage;
}

void Room::setDescription(const std::string& str)
{
    description = str;
}

const std::string &Room::getDescription() const
{
    return description;
}
