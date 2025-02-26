//
// Created by Mario on 25/05/2023.
//

#include "Attribute.h"

#include <utility>

Attribute::Attribute(std::string name, unsigned int value) :
    name(std::move(name)),
    value(value)
{}

Attribute::Attribute(const Attribute &other) :
    name(other.name),
    value(other.value)
{}

void Attribute::increaseValueBy(unsigned int increase)
{
    value += increase;
}

bool Attribute::decreaseValueBy(unsigned int decrease)
{
    if( decrease > value )
        return false;

    value -= decrease;

    return true;
}

unsigned int Attribute::getValue() const
{
    return value;
}

void Attribute::setValue(unsigned int n)
{
    value = n;
}

Attribute::~Attribute() = default;
