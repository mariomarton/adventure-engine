//
// Created by Mário on 19/06/2023.
//

#ifndef SRC_TEXTS_H
#define SRC_TEXTS_H

#include <string>
#include <vector>

/**
 * @struct Texts
 * Contains game texts loaded from JSON configs.
 * */
struct Texts
{
    std::string stringWrongInput;
    std::string stringGameOver;
    std::string stringYes;
    std::string stringNo;

    std::string commandForAdding;
    std::string commandForRemoving;
    std::string commandForCombat;
    std::string commandForEquip;
    std::string commandForUnequip;

    std::string stringSave;
    std::string stringLoad;

    std::vector<std::string> attributeNames;
    std::vector<std::string> namesForRoom;

    std::vector<std::string> stringsForCombat;
    std::vector<std::string> stringsToBeIgnored;
    std::vector<std::string> stringsForIntro;
    std::vector<std::string> stringsForInventory;
};

#endif //SRC_TEXTS_H