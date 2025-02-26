//
// Created by Mario on 26/05/2023.
//

#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iterator>
#include "Game.h"
#include "Util/IO.h"
#include "../libs/json.hpp"


Game::Game(std::string path) :
    pathToGame(std::move(path)),
    isRunning(false),
    currentRoomIndex(0),
    numberOfRooms(0)
{}


void Game::startGame()
{
    if(rooms.empty()){
        std::runtime_error("Cannot launch the game.");
        return;
    }

    isRunning = true;

    runIntro();
    runGame();
}

void Game::runIntro()
{
    using std::cout;
    using std::cin;
    using std::endl;

    std::string introText = texts.stringsForIntro[0] + '\n' + texts.stringsForIntro[1] + '\n';

    int count = 1;

    introText += '\n';

    for(auto& type : mainCharacterTypes)
    {
        introText += (std::to_string(count++)) + "  -  " + type->name + '\n';
    }

    introText += "\n\n";

    print(introText);

    unsigned int typeSelection = 0;

    getSingleNumberInput(typeSelection, 1, mainCharacterTypes.size());

    --typeSelection; // because we want the index

    // At this point, `typeSelection` contains a valid Type index, so we can set the Type
    mainCharacter->setType(mainCharacterTypes[typeSelection]);

    print(texts.stringsForIntro[2] + '\n');

    unsigned int remainingCoins = playerMaxInfluence;

    int attrIndex = 0;

    for(auto& attribute : mainCharacter->getAttributes())
    {
        print(attribute->name + " : ");
        unsigned int attributeSpending = 0;
        getSingleNumberInput(attributeSpending, 0, remainingCoins);
        remainingCoins -= attributeSpending;

        // Now we'll save the value
        attribute->increaseValueBy(attributeSpending);

        // Now we still need to apply the attribute changes caused by the selected Type
        unsigned int attributeIncreaseDueToType = mainCharacter->getType()->getAttributeIncreases()[attrIndex++];
        attribute->increaseValueBy(attributeIncreaseDueToType);

    }

    print(texts.stringsForIntro[3] + "\n\n");

    // Intro completed.
}

std::shared_ptr<GameEntity> Game::findEntity(const std::string& noun) const
{
    auto it = entities.find(noun);
    if (it != entities.end()) {
        return it->second;  // Return the entity if noun is found
    }
    return nullptr;  // Return nullptr if noun not found
}


bool Game::findCommand(const std::string& verbFromInput, std::string& strForCommand) const
{
    // Check if the verb exists in the keys (the main expressions for the commands)
    auto keyIt = commands.find(verbFromInput);
    if (keyIt != commands.end()) {
        //std::cout << "Main expression for " << verbFromInput << ": " << keyIt->first << std::endl;
        strForCommand = keyIt->first;
        return true;
    }

    // Check if the verb exists in the values (the synonyms for the commands)
    for (const auto& entry : commands) {
        const std::vector<std::string>& synonyms = entry.second;
        if (std::find(synonyms.begin(), synonyms.end(), verbFromInput) != synonyms.end()) {
            //std::cout << "Main expression for " << verbFromInput << ": " << entry.first << std::endl;
            strForCommand = entry.first;
            return true;
        }
    }

    return false;
}


void Game::runGame()
{
    print(rooms[currentRoomIndex]->getDescription() + "\n\n");

    while(isRunning){
        runRoom();
    }
}

void Game::runRoom()
{

    std::string verb; // from input
    std::string noun; // from input

    std::string theCommand; // the command that the player actually meant (not a synonym)

    std::shared_ptr<GameEntity> entity = nullptr;

    bool correctInput = false;

    while(!correctInput)
    {
        getVerbAndNounFromInput(verb, noun);

        if(verb == texts.stringsForInventory[0])
        {
            print("\n" + inventory.toString() + "\n\n");
            return;
        }

        if(verb == texts.stringSave)
        {
            saveGame();
            return;
        }

        if(verb == texts.stringLoad)
        {
            loadGame();
            return;
        }

        if(findCommand(verb, theCommand))
        {

            if(wordRefersToRoom(noun))
            {
                print(rooms[currentRoomIndex]->getDescription() + "\n\n");
                return;
            }

            entity = findEntity(noun);

            bool entityInInventory = false;
            bool entityInRoom = false;
            bool entityIsMainCharacter = false;

            // if entity is not an object in current room storage
            // and if it's not in inventory
            // and if it's not an NPC in the current room
            // then it's invalid

            if(entity)
            {
                if(rooms[currentRoomIndex]->getRoomStorage().getEntityCount(entity) > 0) // if it's either an object or an NPC in Room
                    entityInRoom = true;
                else if(inventory.getEntityCount(entity) > 0)
                    entityInInventory = true;
                else if(entity.get() == mainCharacter.get())
                    entityIsMainCharacter = true;
            }

            if(entityInInventory || entityInRoom || entityIsMainCharacter)
            {

                if(theCommand == texts.commandForCombat && entityInRoom)
                {
                    bool combatSuccessful = launchCombat(entity);

                    if(!combatSuccessful)
                        continue; // wrong input
                    else
                        return;
                }
                else if(theCommand == texts.commandForAdding && entityInRoom)
                {
                    if(entity->canBeAddedToInventory())
                    {
                        // add to inventory
                        bool addedToInventory = inventory.add(entity);

                        if(addedToInventory)
                        {
                            std::string theName = entity->name;
                            theName[0] = std::toupper(theName[0]); // capitalise the first letter of the name

                            rooms[currentRoomIndex]->getRoomStorage().removeOne(entity);
                            print("\n" + entity->name + " " + texts.stringsForInventory[1] + "\n\n");
                        } else {
                            print("\n" + texts.stringsForInventory[3] + "\n\n");
                        }

                        return;
                    }
                    // else, incorrect input, get input again

                }
                else if(theCommand == texts.commandForRemoving && entityInInventory)
                {
                            // remove from inventory
                            bool removedFromInventory = inventory.removeOne(entity);

                            if(removedFromInventory)
                            {
                                std::string theName = entity->name;
                                theName[0] = std::toupper(theName[0]); // capitalise the first letter of the name

                                print("\n" + entity->name + " " + texts.stringsForInventory[2] + "\n\n");
                                removeEntityFromMap(*entity);
                                return;
                            }

                            // not supporting ext. storages for now
                }
                else if(theCommand == texts.commandForEquip && entityInInventory)
                {
                    if(inventory.equip(entity))
                        print("\n" + texts.stringsForInventory[4] + "\n\n");
                    else
                        print("\n" + texts.stringsForInventory[5] + "\n\n");

                    return;
                }
                else if(theCommand == texts.commandForUnequip && entityInInventory)
                {
                    if(inventory.unequip(entity))
                        print("\n" + texts.stringsForInventory[4] + "\n\n");
                    else
                        print("\n" + texts.stringsForInventory[5] + "\n\n");
                    return;
                }
                else
                    correctInput = true; // command is not one of the special ones, continue processing
            }
            else
                correctInput = false; // entity not found
        }

        if(!correctInput)
            print("\n" + texts.stringWrongInput + "\n\n");
    }

    try {
        std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> &response = entity->getActionResponse(
                theCommand);

        const std::string &responseText = std::get<0>(response);
        const std::string &responseAltText = std::get<1>(response);
        std::vector<unsigned int> &responseRequirements = std::get<2>(response);
        int responseRoomChange = std::get<3>(response);
        int responseStagesUpdate = std::get<4>(response);

        bool requirementsMet = true;
        if (responseRequirements.size() == texts.attributeNames.size()) {
            requirementsMet = mainCharacter->meetsRequirements(responseRequirements);
        }

        // if character meets requirements, print main text
        if (requirementsMet)
            print("\n" + responseText + "\n\n");
        else
            print("\n" + responseAltText + "\n\n");

        // if specified, update stages of entities
        if (responseStagesUpdate >= 1) {

            unsigned int eventIdxInVector = 0;
            bool found = false;

            // Find the event with the specified ID
            for (eventIdxInVector = 0; eventIdxInVector < stagesUpdatesEvents.size(); ++eventIdxInVector)
            {
                if (stagesUpdatesEvents[eventIdxInVector]->id == responseStagesUpdate) {
                    found = true;
                    break;
                }
            }

            // Trigger the specified event, if found
            if(found)
                triggerStagesUpdates(*stagesUpdatesEvents[eventIdxInVector]);
        }

        // if specified, change room
        if (responseRoomChange >= 1) {

            auto newRoomUnsigned = static_cast<unsigned int>(responseRoomChange); //safe as it's positive

            --newRoomUnsigned; // because we want the index (it's still >=0)

            if(newRoomUnsigned >= rooms.size())
                throw std::runtime_error("The game experienced an internal error.");
            else{
                currentRoomIndex = newRoomUnsigned;
                print(rooms[currentRoomIndex]->getDescription() + "\n\n");
            }
        }

        // if specified, end the game (room change value negative)
        if (responseRoomChange < 0) {
            isRunning = false;
        }
    }
    catch (std::invalid_argument &) {
        print("\n" + texts.stringWrongInput + "\n\n");
    }

}

bool Game::ignoreThisWord(const std::string& word)
{
    auto it = find(texts.stringsToBeIgnored.begin(), texts.stringsToBeIgnored.end(), word);
    if (it != texts.stringsToBeIgnored.end())
        return true;

    return false;
}

void Game::getVerbAndNounFromInput(std::string& verb, std::string& noun)
{
    using namespace std;

    string userInput;
    bool validInput = false;

    while (!validInput)
    {
        getline(cin, userInput);  // Read the entire line of input

        // Convert input to lowercase
        transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

        // Remove punctuation marks and symbols
        userInput.erase(remove_if(userInput.begin(), userInput.end(), [](char c) {
            return ispunct(c);  // Remove punctuation
        }), userInput.end());

        // Get words (= tokens) from the input
        vector<string> words;
        istringstream iss(userInput);
        copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));

        // Process the words
        verb.clear();
        noun.clear();

        for (const string& word : words)
        {
            if( ignoreThisWord(word) )
                continue;

            if (verb.empty())
                verb = word;
            else if (noun.empty())
                noun = word;
        }

        if (!verb.empty())
        {
            validInput = true;
        }
        else
        {
            print("\n" + texts.stringWrongInput + "\n\n");
        }
    }

    // Verb & noun strings now contain the correct input.
}



std::unordered_map<std::string, std::vector<std::string>>& Game::getCommandsMap()
{
    return commands;
}

void Game::saveGame()
{
    nlohmann::json gameData;

    unsigned int typeIndex = 0;
    std::vector<unsigned int> attributeValues;

    // Save MC type
    for (auto& type : mainCharacterTypes)
    {
        if (type == mainCharacter->getType())
            break;

        ++typeIndex;
    }

    // Save attribute values
    auto& attributes = mainCharacter->getAttributes();

    attributeValues.reserve(attributes.size());

    for (auto& attr : attributes)
    {
        attributeValues.push_back(attr->getValue());
    }

    // Store the values in the gameData JSON object
    gameData["currentRoomIndex"] = currentRoomIndex;
    gameData["typeIndex"] = typeIndex;
    gameData["attributeValues"] = attributeValues;

    std::string savePath = pathToGame + "/saves/game_save.json";

    std::ofstream file(savePath, std::ios::trunc);
    if (file.is_open())
    {
        file << gameData;
        file.close();
    }
    else
    {
        std::cerr << "\nCould not save the game.\n\n" << std::endl;
    }

    print("\nThe game has been saved.\n\n");
}


void Game::setPlayerMaxInfluence(unsigned int maxInfluence)
{
    playerMaxInfluence = maxInfluence;
}

InventoryStorage& Game::getInventory()
{
    return inventory;
}

void Game::setNumberOfRooms(unsigned int n)
{
    numberOfRooms = n;
}

void Game::addStagesUpdatesEvent(std::unique_ptr<StagesUpdatesEvent> event)
{
    stagesUpdatesEvents.push_back(std::move(event));
}

void Game::setAnimationSpeed(int speed)
{
    printAnimationSpeed = speed;
}

bool Game::addNamesToEntityMap(const std::vector<std::string>& names, std::shared_ptr<GameEntity> entity)
{
    for (const std::string& name : names)
    {
        // Add the key-value pair to the entities map
        if (!entities.insert({ name, entity }).second)
        {
            throw std::runtime_error("The game cannot be launched. There are invalid entities in the conf files.");
            return false;
        }
    }
    return true;
}

void Game::setMainCharacter(std::shared_ptr<MainCharacter> character)
{
    mainCharacter = std::move(character);
}

void Game::triggerStagesUpdates(StagesUpdatesEvent& event)
{
    std::map<std::string, unsigned int>& changesMap = event.getChangesMap();

    // Iterate over map pair
    for (const auto& pair : changesMap)
    {
        const std::string& entityName = pair.first;


        unsigned int newStage = pair.second;

        std::shared_ptr<GameEntity> entity = findEntity(entityName);

        if(!entity)
        {
            // Error -> Cannot access one of the inner entities.
            // But we'll let the game continue.
            return;
        }

        if(!entity->setStageTo(newStage))
        {
            // Error -> unable to change a stage to the given value, the value is invalid.
            // But we'll let the game continue.
            return;
        }
    }

    // The stages are now updated.
}

bool Game::launchCombat(std::shared_ptr<GameEntity> npc)
{
    //NPC has   - Health (unsigned)
    //          - CombatStats (int) : usesRemaining, HitPower, HitSpeed

    //MC has    - Health Attribute  (-> value : unsigned)
    //          - Power Attribute   (-> value : unsigned)
    //          - Weapons in InventoryStorage, maybe   (entity->combatStats != nullptr)
    //                  - CombatStats (int) : usesRemaining, HitPower, HitSpeed

    //First, we call prepareForCombatAndGetPower() on MainCharacter
        // This returns the MPC power, and also updates its usingWeapon variable

    int powerMC = mainCharacter->prepareForCombatAndGetPower();
    std::shared_ptr<GameEntity> theWeapon = mainCharacter->getWeaponInUse();

    //Now let's get the total power of the NPC
    int powerNPC = npc->prepareForCombatAndGetPower();

    // Time to reduce health of MC and NPC
    unsigned int npcHealth = 0;

    try
    {
        npcHealth = npc->getHealthAfterDecrease(powerMC);
    }
    catch(const std::invalid_argument& e)
    {
        return false;
    }

    unsigned int mcHealth = mainCharacter->getHealthAfterDecrease(powerNPC);

    bool playerSurvived = false;
    bool npcSurvived = false;

    //let's determine the result and possibly update the weapons or the npc
    if(npcHealth > 0)
        npcSurvived = true;

    if(mcHealth > 0)
        playerSurvived = true;

    print("\n\n");

    //print something like "You've fought with $ENEMY_NAME using $WEAPON_NAME." (or no weapon name if appropriate)
    if(!theWeapon)
        print(texts.stringsForCombat[0] + " " + npc->name + ".");
    else
        print(texts.stringsForCombat[0] + " " + npc->name + " " + texts.stringsForCombat[1] + " " + theWeapon->name + ".");

    //print something like "Your health is at $VALUE %."
    print(texts.stringsForCombat[2] + " " + std::to_string(mcHealth) + "%.");

    //print something like "The health of $ENEMY_NAME at $VALUE %."
    print(texts.stringsForCombat[4] + " " + npc->name + " " + texts.stringsForCombat[5] + " " + std::to_string(npcHealth) + "%.");


    if(playerSurvived && !npcSurvived)
    {
        // player won

        // remove the NPC from the entites
        removeEntityFromMap(*npc);

        // update the weapon the MC was using
        if(theWeapon)
            theWeapon->decreaseUsesRemaining();

        //print something like "You defeated $ENEMY_NAME, it's dead."
        print(texts.stringsForCombat[6] + " " + npc->name + " " + texts.stringsForCombat[7] + ".");
    }
    else if(playerSurvived && npcSurvived)
    {
        // both survived

        // update the weapon the MC was using
        if(theWeapon)
            theWeapon->decreaseUsesRemaining();

        // update NPC combatStats
        npc->decreaseUsesRemaining();

        //print something like "You both survived. You can now attack again."
        print(texts.stringsForCombat[8]);
    }
    else
    {
        // player is dead, game over
        isRunning = false;

        //print something like "The enemy finished you, you're dead"
        print(texts.stringsForCombat[3]);
    }
    print("\n\n");
    return true;
}

void Game::removeEntityFromMap(GameEntity& entityToBeRemoved)
{
    for (auto it = entities.begin(); it != entities.end();)
    {
        if ((it->second).get() == &entityToBeRemoved)
        {
            // Remove the key-value pair from the map
            it = entities.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

std::vector<std::shared_ptr<MainCharacterType> > &Game::getMainCharacterTypes() {
    return mainCharacterTypes;
}

void Game::getSingleNumberInput(unsigned int& variableForInput, unsigned int min, unsigned int max)
{
    readSingleNumberInput(variableForInput, min, max, texts.stringWrongInput, printAnimationSpeed); // from the IO file
}

bool Game::wordRefersToRoom(const std::string& word)
{
    auto it = std::find(texts.namesForRoom.begin(), texts.namesForRoom.end(), word);
    return it != texts.namesForRoom.end(); // Return true if the word is found, false otherwise
}

void Game::loadGame()
{
    std::string savePath = pathToGame + "/saves/game_save.json";

    std::ifstream file(savePath);
    if (file.is_open())
    {
        nlohmann::json gameData;
        file >> gameData;
        file.close();

        // Load the values from the gameData JSON object

        unsigned int roomIdx = gameData["currentRoomIndex"];
        unsigned int typeIndex = gameData["typeIndex"];
        std::vector<unsigned int> attributeValues = gameData["attributeValues"];

        // Set the loaded values in the game state

        if (roomIdx < rooms.size())
            currentRoomIndex = roomIdx;

        if (typeIndex < mainCharacterTypes.size())
            mainCharacter->setType(mainCharacterTypes[typeIndex]);

        auto& attributes = mainCharacter->getAttributes();

        if (attributeValues.size() == attributes.size())
        {
            for (size_t i = 0; i < attributes.size(); ++i)
            {
                attributes[i]->setValue(attributeValues[i]);
            }
        }
        else
        {
            // wrong attributes object
            std::cerr << "Could not load the game." << std::endl;
            return;
        }

        print("\nThe game has been loaded.\n\n");
        print(rooms[currentRoomIndex]->getDescription() + "\n\n");
    }
    else
    {
        std::cerr << "Could not load the game." << std::endl;
    }
}

void Game::addRoom(std::unique_ptr<Room> room)
{
    if (room)
        rooms.push_back(std::move(room));
    else
        throw std::runtime_error("The game cannot be launched.");
}

void Game::print(const std::string& text)
{
    animateText(text, printAnimationSpeed); // from the IO file
}

