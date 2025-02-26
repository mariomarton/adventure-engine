//
// Created by Mario on 25/05/2023.
//

#include "MainCharacterType.h"

MainCharacterType::MainCharacterType(const std::string &name) :
    name(name)
{
    attributeIncreases = std::vector<unsigned int>();
}

MainCharacterType::MainCharacterType(const MainCharacterType &other) :
    name(other.name),
    attributeIncreases(std::vector<unsigned int>(other.attributeIncreases))
{}

MainCharacterType::~MainCharacterType() = default;

std::vector<unsigned int>& MainCharacterType::getAttributeIncreases()
{
    return attributeIncreases;
}
