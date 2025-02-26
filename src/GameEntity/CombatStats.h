//
// Created by Mário on 05/06/2023.
//

#ifndef SRC_COMBATSTATS_H
#define SRC_COMBATSTATS_H

/**
 * @class CombatStats
 * A series of values used for combat.
 * CombatStats are required for weapons of the Main Characters and for NPCs.
 * */
struct CombatStats
{
    CombatStats(int remaining, int power, int speed)
    {
        usesRemaining = remaining;
        hitPower = power;
        hitSpeed = speed;
    }

    /** How many more times can the Game Entity take part in combat? */
    int usesRemaining;

    /**
     * Power of the entity relative to the main character with no weapon.
     * It's a rounded percentage value, hitPower of 100 means it's equal to MC without a weapon.
     * */
    int hitPower;

    /**
     * Similarly to hitPower, hitSpeed is also relative to the main character with no weapon.
     * It represents the number of hits the entity does in one combat event.
     * It's a rounded percentage value, hitSpeed of 100 means it's equal to MC without a weapon.
     * */
    int hitSpeed;
};

#endif //SRC_COMBATSTATS_H
