//
// Created by Mario on 26/05/2023.
//

#include "Reader.h"
#include <fstream>
#include <iostream>
#include <utility>
#include "../GameEntity/NPC.h"
#include "../Inventory/InventoryStorage.h"
#include "../Events/StagesUpdatesEvent.h"
#include "../TypeAndAttribute/Attribute.h"

using json = nlohmann::json;

Reader::Reader(std::string directory, Game& game) :
    game(game),
    directory(std::move(directory))
{}

bool Reader::readEverything()
{
    if (!readCommands()) {
        std::cerr << "Failed to read commands." << std::endl;
        return false;
    }

    if (!readTypesAndAttributes()) {
        std::cerr << "Failed to read types and attributes." << std::endl;
        return false;
    }

    if (!readInventoryStorage()) {
        std::cerr << "Failed to read inventory." << std::endl;
        return false;
    }

    if (!readRooms()) {
        std::cerr << "Failed to read rooms." << std::endl;
        return false;
    }

    if (!readStagesUpdates()) {
        std::cerr << "Failed to read stages updates." << std::endl;
        return false;
    }

    if (!readCombat()) {
        std::cerr << "Failed to read combat." << std::endl;
        return false;
    }

    if (!readEssentialTextsAndValues()) {
        std::cerr << "Failed to read essential texts and values." << std::endl;
        return false;
    }

    if (!readMainCharacter()) {
        std::cerr << "Failed to read main character." << std::endl;
        return false;
    }

    return true;
}

void Reader::toLowercase(std::string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

std::string Reader::getPath(const std::string& fileName, const std::string& subDirectory)
{
    if(!subDirectory.empty())
        return directory + separator + subDirectory + separator + fileName;
    else return directory + separator + fileName;
}

std::string Reader::getPathForRoom(unsigned int number)
{
    return directory + separator + std::string(directoryNameForRooms) + separator
           + std::string(fileNameForRoomNoExtension) + std::to_string(number) + ".json";
}

int Reader::readIntoJsonFromFile(const std::string& fileName, json& root)
{
    // Read the JSON file
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return ERROR_CANNOT_OPEN_FILE;
    }

    // Parse the JSON data
    try {
        file >> root;
    } catch (const json::parse_error& e) {
        return ERROR_JSON_INVALID;
    }

    return 0;
}

bool Reader::readCommands()
{
    json root;
    std::string path = getPath(std::string(fileNameForCommands));

    int jsonRead = readIntoJsonFromFile(path, root);
    if(jsonRead != 0)
    {
        if(jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr<<"CANNOT OPEN FILE "<<fileNameForCommands<<std::endl;
            std::cerr<<"GIVEN FILE PATH: "<<path<<std::endl;
            return false;
        }
        else
        {
            std::cerr<<"JSON INVALID -> "<<fileNameForCommands<<std::endl;
            return false;
        }
    }

    // Read commands
    if (!root.contains(keywordForCommands))
    {
        std::cerr << "The key \""<<keywordForCommands<<"\" is not present in the JSON, the file is invalid -> " << fileNameForCommands
                  << std::endl;
        return false;
    }

    const json& commands = root[keywordForCommands];

    // Read commands
    if (!commands.is_object())
    {
        std::cerr << "The commands in the file must form a valid JSON object but they don't,"
                     "the file is invalid -> " << fileNameForCommands
                  << std::endl;
        return false;
    }

    for (json::const_iterator it = commands.begin(); it != commands.end(); ++it)
    {
        std::string command = it.key();
        const json& synonyms = it.value();

        toLowercase(command);

        if (!synonyms.is_array())
        {
            std::cerr<<"JSON INVALID - -> "<<fileNameForCommands<<". Synonyms for a command are not an array."<<std::endl;
            return false;
        }



        for (const auto& synonym : synonyms) {
            if(!synonym.is_string())
            {
                std::cerr<<"JSON INVALID - -> "<<fileNameForCommands<<". A synonym is not a string."<<std::endl;
                return false;
            }

            std::string synonymStr = synonym.get<std::string>();

            toLowercase(synonymStr);

            game.getCommandsMap()[command].push_back(synonymStr);
        }
    }

    if (!root.contains(keywordForCommandForAdding)       ||
        !root.contains(keywordForCommandForRemoving)   ||
        !root.contains(keywordForCommandForEquip)   ||
        !root.contains(keywordForCommandForUnequip)   ||
        !root.contains(keywordForCommandForCombat))
    {
        std::cerr << "JSON INVALID. It is missing a command (for adding/removing from inventory, or for combat). "
                     "Add it to this file -> " << fileNameForCommands << std::endl;
        return false;
    }

    const json& commandForAdding    = root[keywordForCommandForAdding];
    const json& commandForRemoving  = root[keywordForCommandForRemoving];
    const json& commandForCombat    = root[keywordForCommandForCombat];
    const json& commandForEquip     = root[keywordForCommandForEquip];
    const json& commandForUnequip   = root[keywordForCommandForUnequip];

    if( !commandForAdding.is_string()   ||
        !commandForRemoving.is_string() ||
        !commandForEquip.is_string() ||
        !commandForUnequip.is_string() ||
        !commandForCombat.is_string() )
    {
        std::cerr<<"JSON INVALID - -> "<<fileNameForCommands<<". A command is not a string."<<std::endl;
        return false;
    }

    std::string cmdForAdding = commandForAdding.get<std::string>();
    std::string cmdForRemoving = commandForRemoving.get<std::string>();
    std::string cmdForCombat = commandForCombat.get<std::string>();
    std::string cmdForEquip = commandForEquip.get<std::string>();
    std::string cmdForUnequip = commandForUnequip.get<std::string>();

    toLowercase(cmdForAdding);
    toLowercase(cmdForRemoving);
    toLowercase(cmdForCombat);
    toLowercase(cmdForEquip);
    toLowercase(cmdForUnequip);

    game.texts.commandForAdding     = cmdForAdding;
    game.texts.commandForRemoving   = cmdForRemoving;
    game.texts.commandForEquip      = cmdForEquip;
    game.texts.commandForUnequip    = cmdForUnequip;
    game.texts.commandForCombat     = cmdForCombat;

    return true;
}

bool Reader::readTypesAndAttributes()
{
    json root;
    std::string path = getPath(std::string(fileNameForTypesAndAttributes));

    int jsonRead = readIntoJsonFromFile(path, root);
    if(jsonRead != 0)
    {
        if(jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr<<"CANNOT OPEN FILE "<<fileNameForTypesAndAttributes<<std::endl;
            return false;
        }
        else
        {
            std::cerr<<"JSON INVALID -> "<<fileNameForTypesAndAttributes<<std::endl;
            return false;
        }
    }

    // Read attributes

    if (!root.contains(keywordForAttributes) ||
        !root.contains(keywordForCharacterTypes) ||
        !root.contains(keywordForN) )
    {
        std::cerr   << "A key is missing in the JSON, the file is invalid -> " << fileNameForTypesAndAttributes
                    << std::endl;

        std::cerr   << "Make sure the file includes these keys: "<<keywordForAttributes
                    <<", "<<keywordForCharacterTypes<<" and "<<keywordForN<<"."<<std::endl;
        return false;
    }

    const json& attributes = root[keywordForAttributes];
    const json& playerMaxInfluence = root[keywordForN];
    const json& characterTypes = root[keywordForCharacterTypes];

    std::vector<std::string>& attributeNames = game.texts.attributeNames;

    if (!attributes.is_array()                      ||
        !playerMaxInfluence.is_number_unsigned()    ||
        !characterTypes.is_object()                 ||
        characterTypes.empty()
    )
    {
        std::cerr      <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes<<"."
                         "A key in this file has an invalid value."<<std::endl;
        return false;
    }

    // Iterate over the JSON array and extract the values
    for (const auto& attribute : attributes) {

        if(!attribute.is_string())
        {
            std::cerr   <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes
                        <<". An attribute is not a string."<<std::endl;
            return false;
        }

        std::string attributeValue = attribute.get<std::string>();
        //toLowercase(attributeValue);
        attributeNames.push_back(attributeValue);
    }

    const unsigned int playerMaxInfluenceValue = playerMaxInfluence.get<unsigned int>();

    game.setPlayerMaxInfluence(playerMaxInfluenceValue);

    // Read character types
    auto& gameCharacterTypesVector = game.getMainCharacterTypes();

    for (json::const_iterator it = characterTypes.begin(); it != characterTypes.end(); ++it)
    {
        const std::string& characterType = it.key();
        const json& attributeIncreases = it.value();

        if(!attributeIncreases.is_array())
        {    std::cerr      <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes
                            <<". Attribute-increases don't form an array."<<std::endl;
             return false;
        }

        auto thisType = std::make_shared<MainCharacterType>(characterType); // will be deleted in Game


        auto& attributeIncreasesVector = thisType->getAttributeIncreases();
        gameCharacterTypesVector.push_back(thisType);

        for (const auto& attributeIncrease : attributeIncreases)
        {

            if(!attributeIncrease.is_number_unsigned()){
                std::cerr   <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes
                            <<". An attribute-increase value is not an integer."<<std::endl;
                return false;
            }

            attributeIncreasesVector.push_back( attributeIncrease.get<unsigned int>() );
        }

        if(attributeIncreasesVector.size() != attributeNames.size())
        {
            std::cerr   <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes
                        <<". The number of attribute-increases doesn't match the number of attributes."<<std::endl;
            return false;
        }
    }

    return true;
}

bool Reader::readInventoryStorage()
{
    json root;
    std::string path = getPath(std::string(fileNameForInventoryStorage), std::string(directoryNameForInventory));

    int jsonRead = readIntoJsonFromFile(path, root);
    if(jsonRead != 0)
    {
        if(jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr<<"CANNOT OPEN FILE "<<fileNameForInventoryStorage<<std::endl;
            return false;
        }
        else
        {
            std::cerr<<"JSON INVALID -> "<<fileNameForInventoryStorage<<std::endl;
            return false;
        }
    }

    // Read Max Capacity
    const json& maxCapacity = root[keywordForCapacity];
    const json& objects = root[keywordForObjects];

    if( !maxCapacity.is_number_unsigned() || !objects.is_object() )
    {
        std::cerr   <<  "JSON INVALID -> "<<fileNameForInventoryStorage
                    <<  ". Make sure that max. capacity is a positive number "
                        " and that the storage's objects form a valid JSON object."<<std::endl;
        return false;
    }

    unsigned int maxCapacityValue = maxCapacity.get<unsigned int>();
    auto& mainCharacterStorage = game.getInventory();
    mainCharacterStorage.setSlotsTotal(maxCapacityValue);


    // Now to reading objects
    if(!readEntities(std::string(fileNameForInventoryStorage), mainCharacterStorage, objects, false, false))
        return false;

    // Objects have been read, continue

    if(mainCharacterStorage.getSlotsUsed() > mainCharacterStorage.getSlotsTotal())
    {
        std::cerr   <<  "LOGIC INVALID IN THIS FILE -> "<<fileNameForInventoryStorage
                    <<  ". Storage capacity exceeded."<<std::endl;
        return false;
    }

    // Read Object in Left and Right Hand
    if(!root.contains(keywordForObjectInLeftHand) || !root.contains(keywordForObjectInRightHand))
    {
        std::cerr   <<  "JSON INVALID -> "<<fileNameForInventoryStorage
                    <<  ". Object in Left/Right hand key-value pair missing."<<std::endl;
        return false;
    }

    const json& leftHand = root[keywordForObjectInLeftHand];
    const json& rightHand = root[keywordForObjectInRightHand];

    if( ! ( (leftHand.is_string() || leftHand.is_null()) && ((rightHand.is_string() || rightHand.is_null())) ) )
    {
        std::cerr   <<  "JSON INVALID -> "<<fileNameForInventoryStorage
                    <<  ". Object in Left/Right hand - the value must be either null or a string."<<std::endl;
        return false;
    }

    mainCharacterStorage.clearHands();

    if(!leftHand.is_null()){
        std::string leftHandValue = leftHand.get<std::string>();

        toLowercase(leftHandValue);

        auto objectL = mainCharacterStorage.getEntityFromName(leftHandValue);

        if(!objectL)
        {
            std::cerr   <<  "ERROR in file -> "<<fileNameForInventoryStorage
                        <<  ". The object to be put into the left hand is invalid."<<std::endl;
            return false;
        }

        if(!mainCharacterStorage.equip(objectL))
        {
            std::cerr   <<  "ERROR in file -> "<<fileNameForInventoryStorage
                        <<  ". The object to be put into the left hand cannot be put there.."<<std::endl;
            return false;
        }
    }

    if(!rightHand.is_null()){
        std::string rightHandValue = rightHand.get<std::string>();

        toLowercase(rightHandValue);

        auto objectR = mainCharacterStorage.getEntityFromName(rightHandValue);

        if(!objectR)
        {
            std::cerr   <<  "ERROR in file -> "<<fileNameForInventoryStorage
                        <<  ". The object to be put into the right hand is invalid."<<std::endl;
            return false;
        }

        if(!mainCharacterStorage.equip(objectR))
        {
            std::cerr   <<  "ERROR in file -> "<<fileNameForInventoryStorage
                        <<  ". The object to be put into the right hand cannot be put there.."<<std::endl;
            return false;
        }
    }

    return true;
}

bool Reader::readRooms()
{
    //get number of rooms
    json setupRoot;
    std::string setupPath = getPath(std::string(fileNameForGameSetup));

    int jsonRead = readIntoJsonFromFile(setupPath, setupRoot);
    if(jsonRead != 0)
    {
        if(jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr<<"CANNOT OPEN FILE "<<fileNameForGameSetup<<std::endl;
            return false;
        }
        else
        {
            std::cerr<<"JSON INVALID -> "<<fileNameForGameSetup<<std::endl;
            return false;
        }
    }

    if (!setupRoot.contains(keywordForNumberOfRooms))
    {
        std::cerr << "The key \""<<keywordForNumberOfRooms<<"\""
                     " is not present in the JSON,the file is invalid -> " << fileNameForGameSetup << std::endl;
        return false;
    }

    if(!setupRoot[keywordForNumberOfRooms].is_number_unsigned())
    {
        std::cerr << "The value of the \""<<keywordForNumberOfRooms<<"\" key is not"
                     " an unsigned integer, the file is invalid -> " << fileNameForGameSetup << std::endl;
        return false;
    }

    unsigned int numberOfRooms = setupRoot[keywordForNumberOfRooms].get<unsigned int>();

    game.setNumberOfRooms( numberOfRooms );

    //number of rooms set

    //call readOneRoom in a cycle

    for (unsigned int thisRoomIndex = 0; thisRoomIndex < numberOfRooms; ++thisRoomIndex)
    {
        json root;
        std::string path = getPathForRoom(thisRoomIndex + 1);

        int jsonRoomRead = readIntoJsonFromFile(path, root);
        if(jsonRoomRead != 0)
        {
            if(jsonRoomRead == ERROR_CANNOT_OPEN_FILE)
            {
                std::cerr<<"CANNOT OPEN FILE "<<path<<std::endl;
                return false;
            }
            else
            {
                std::cerr<<"JSON INVALID -> "<<path<<std::endl;
                return false;
            }
        }

        auto room = std::make_unique < Room > ();

        readOneRoom(root, std::move(room));
    }

    return true;
}

bool Reader::readStagesUpdates()
{
    //open and load stages-updates.json
    json root;
    std::string path = getPath(std::string(fileNameForStagesUpdates), std::string(directoryNameForEvents));

    int jsonRead = readIntoJsonFromFile(path, root);
    if(jsonRead != 0)
    {
        if(jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr<<"CANNOT OPEN FILE "<<fileNameForStagesUpdates<<std::endl;
            return false;
        }
        else
        {
            std::cerr<<"JSON INVALID -> "<<fileNameForStagesUpdates<<std::endl;
            return false;
        }
    }

    if (!root.contains(keywordForStagesUpdatesMainArray))
    {
        std::cerr << "The key \""<<keywordForStagesUpdatesMainArray<<"\""
                     " is not present in the JSON,the file is invalid -> "
                     << fileNameForStagesUpdates << std::endl;
        return false;
    }

    const json& stagesUpdates = root[keywordForStagesUpdatesMainArray];

    //now make sure the key's value is an array
    if(!stagesUpdates.is_array())
    {    std::cerr      <<"JSON INVALID - -> "<<fileNameForTypesAndAttributes
                        <<". The value of the STAGES-UPDATES EVENTS key must  an array."<<std::endl;
        return false;
    }

    // Iterate over each object in the stagesUpdates array
    for (const auto& object : stagesUpdates) {
        if(!readOneStagesUpdatesObject(object))
            return false;
    }

    return true;
}

bool Reader::readOneStagesUpdatesObject(const json& object)
{
    // Check if object is a JSON object
    if (!object.is_object()) {
        std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                       <<". Every element in the STAGES-UPDATES EVENTS array must be a valid JSON object."<<std::endl;
        return false;
    }

    // Check if it contains "STAGES-UPDATES EVENT ID"
    if (!object.contains(keywordForStagesUpdatesID)) {
        std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                       <<". A STAGES-UPDATES object is missing the EVENT ID key."<<std::endl;
        return false;
    }

    //check if the ID is an unsigned int
    if (!object[keywordForStagesUpdatesID].is_number_unsigned()) {
        std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                       <<". The 'STAGES-UPDATES EVENT ID' is not an unsigned integer."<<std::endl;
        return false;
    }

    //get the id
    int eventID = object[keywordForStagesUpdatesID].get<int>();

    // Check if it contains "ENTITY - STAGE PAIRS"
    if (!object.contains(keywordForStagesUpdatesPairs)) {
        std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                       <<". A STAGES-UPDATES object is missing the ENTITY - STAGE PAIRS key."<<std::endl;
        return false;
    }

    // Check if the Pairs are a valid JSON object
    if(!object[keywordForStagesUpdatesPairs].is_object()){
        std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                       <<". A STAGES-UPDATES object's Pairs are not a valid JSON object."<<std::endl;
        return false;
    }

    auto stagesUpdatesEvent = std::make_unique <StagesUpdatesEvent> (eventID);

    auto& entityAndStageMap = stagesUpdatesEvent->getChangesMap();

    // entityAndStageMap will be deleted in StagesUpdatesEvent
    game.addStagesUpdatesEvent( std::move(stagesUpdatesEvent) );

    // Iterate over each key-value pair in the Pairs object
    const json& pairsObject = object[keywordForStagesUpdatesPairs];
    for (const auto& pair : pairsObject.items())
    {
        const std::string& key = pair.key();
        const json& value = pair.value();

        // Check if the value is an unsigned int
        if (!value.is_number_unsigned()) {
            std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                           <<". In the Pairs of a Stages-Updates object, a value of the new stage is invalid,"
                             " it must be an unsigned integer."<<std::endl;
            return false;
        }

        // Access the string key and integer value
        std::string theKey = key;
        toLowercase(theKey);
        int theStageValue = value.get<unsigned int>();
        --theStageValue; // because of index syntax

        // Insert the key-value pair into the entityAndStageMap and check for errors
        auto insertResult = entityAndStageMap.insert(std::make_pair(theKey, theStageValue));
        if (!insertResult.second) {
            // Insertion failed because the key (GameEntity name) already exists in the map
            std::cerr      <<"JSON INVALID - -> "<<fileNameForStagesUpdates
                           <<". In the Pairs of a Stages-Updates object, there's a duplicate Entity."<<std::endl;
            return false;
        }
    }

    return true;
}

bool Reader::readCombat() {
    // load combat.json

    json root;
    std::string path = getPath(std::string(fileNameForCombat), std::string(directoryNameForEvents));

    int jsonRead = readIntoJsonFromFile(path, root);
    if (jsonRead != 0) {
        if (jsonRead == ERROR_CANNOT_OPEN_FILE) {
            std::cerr << "CANNOT OPEN FILE " << fileNameForCombat << std::endl;
            return false;
        } else {
            std::cerr << "JSON INVALID -> " << fileNameForCombat << std::endl;
            return false;
        }
    }

    const int numOfKeywords = sizeof(keywordCombatText) / sizeof(keywordCombatText[0]);

    for (int i = 0; i < numOfKeywords; i++)
    {
        const std::string &keyword = std::string(keywordCombatText[i]);

        // Check if keyword is present in the JSON and is of type string
        if (root.contains(keyword) && root[keyword].is_string()) {

            // Store the string value in the game's array for this purpose
            game.texts.stringsForCombat.push_back(root[keyword].get<std::string>());
        }
        else {
            // Handle the case where the keyword is missing or not a string
            std::cerr << "JSON INVALID -> " << fileNameForCombat << std::endl
                      << "The following key is missing or it's value is not a string: " << keyword << std::endl;
            return false;
        }
    }

    return true;
}

bool Reader::readEssentialTextsAndValues()
{
    json root;
    std::string path = getPath(std::string(fileNameForGameSetup));

    int jsonRead = readIntoJsonFromFile(path, root);
    if (jsonRead != 0)
    {
        if (jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr << "CANNOT OPEN FILE " << fileNameForGameSetup << std::endl;
            return false;
        }
        else
        {
            std::cerr << "JSON INVALID -> " << fileNameForGameSetup << std::endl;
            return false;
        }
    }

    // Check if all the keys are present

    if (!root.contains(keywordSetupIntroTexts) ||
        !root.contains(keywordSetupInventoryTexts) ||
        !root.contains(keywordSetupWrongInputText) ||
        !root.contains(keywordSetupGameOverText) ||
        !root.contains(keywordSetupWordForYes) ||
        !root.contains(keywordSetupWordForNo) ||
        !root.contains(keywordSetupStringsToBeIgnored) ||
        !root.contains(keywordSetupNamesForRoom))
    {
        std::cerr   << "A key is missing in the JSON, the file is invalid -> " << fileNameForGameSetup << std::endl;
        std::cerr   << "Make sure the file includes these keys: " << keywordSetupIntroTexts << ", "
                    << keywordSetupWrongInputText << ", " << keywordSetupGameOverText << ", " << keywordSetupWordForYes
                    << ", " << keywordSetupWordForNo << ", " << keywordSetupNamesForRoom << ", "
                    << keywordSetupInventoryTexts << ", and " << keywordSetupStringsToBeIgnored << "." << std::endl;
        return false;
    }

    // Check the value types

    if (!root[keywordSetupIntroTexts].is_object() ||
        !root[keywordSetupInventoryTexts].is_object() ||
        !root[keywordSetupWrongInputText].is_string() ||
        !root[keywordSetupGameOverText].is_string() ||
        !root[keywordSetupWordForYes].is_string() ||
        !root[keywordSetupWordForNo].is_string() ||
        !root[keywordSetupStringsToBeIgnored].is_array() ||
        !root[keywordSetupNamesForRoom].is_array())
    {
        std::cerr <<    "JSON INVALID - -> " << fileNameForGameSetup <<
                        ". A key in this file has an invalid value." << std::endl;
        return false;
    }

    const json& introTexts = root[keywordSetupIntroTexts];
    const json& inventoryTexts = root[keywordSetupInventoryTexts];
    const json& stringsToBeIgnored = root[keywordSetupStringsToBeIgnored];
    const json& namesForRoom = root[keywordSetupNamesForRoom];

    auto& ignoreStringsVector = game.texts.stringsToBeIgnored;
    auto& namesForRoomVector = game.texts.namesForRoom;

    for (const auto& element : stringsToBeIgnored)
    {
        if (!element.is_string())
        {
            std::cerr   << "JSON INVALID - -> " << fileNameForGameSetup << ". An element in the \""
                        << keywordSetupStringsToBeIgnored << "\" array is not a string." << std::endl;
            return false;
        }

        std::string strToBeIgnored = element.get<std::string>();

        toLowercase(strToBeIgnored);

        ignoreStringsVector.push_back(strToBeIgnored);
    }

    // strings to be ignored - now done, now load names for room

    for (const auto& element : namesForRoom)
    {
        if (!element.is_string())
        {
            std::cerr   << "JSON INVALID - -> " << fileNameForGameSetup << ". An element in the \""
                        << keywordSetupNamesForRoom << "\" array is not a string." << std::endl;
            return false;
        }

        //
        std::string nameForRoom = element.get<std::string>();

        toLowercase(nameForRoom);

        namesForRoomVector.push_back(nameForRoom);
    }

    // names for room now also done

    if (!introTexts.contains(keywordSetupIntro1) ||
        !introTexts.contains(keywordSetupIntro2) ||
        !introTexts.contains(keywordSetupIntro3) ||
        !introTexts.contains(keywordSetupIntro4))
    {
        std::cerr   << "A key is missing in the \"INTRO TEXTS\" object, the file is invalid -> "
                    << fileNameForGameSetup << std::endl;
        return false;
    }

    if (!introTexts[keywordSetupIntro1].is_string() ||
        !introTexts[keywordSetupIntro2].is_string() ||
        !introTexts[keywordSetupIntro3].is_string() ||
        !introTexts[keywordSetupIntro4].is_string())
    {
        std::cerr   << "JSON INVALID - -> " << fileNameForGameSetup
                    << ". A value in the \"INTRO TEXTS\" object has an invalid type." << std::endl;
        return false;
    }

    // Now let's process all the remaining values

    game.texts.stringWrongInput = root[keywordSetupWrongInputText].get<std::string>();
    game.texts.stringGameOver = root[keywordSetupGameOverText].get<std::string>();
    game.texts.stringYes = root[keywordSetupWordForYes].get<std::string>();
    game.texts.stringNo = root[keywordSetupWordForNo].get<std::string>();

    std::vector<std::string>& introStringsVector = game.texts.stringsForIntro;
    introStringsVector.push_back(std::string(introTexts[keywordSetupIntro1].get<std::string>()));
    introStringsVector.push_back(std::string(introTexts[keywordSetupIntro2].get<std::string>()));
    introStringsVector.push_back(std::string(introTexts[keywordSetupIntro3].get<std::string>()));
    introStringsVector.push_back(std::string(introTexts[keywordSetupIntro4].get<std::string>()));


    constexpr int numInventoryKeywords = sizeof(keywordSetupInventory) / sizeof(keywordSetupInventory[0]);
    std::vector<std::string>& inventoryStringsVector = game.texts.stringsForInventory;

    for (int i = 0; i < numInventoryKeywords; i++)
    {
        if (inventoryTexts.contains(keywordSetupInventory[i]) && inventoryTexts[keywordSetupInventory[i]].is_string())
        {
            if(i ==0){
                //this is the keyword for inventory - it acts like a command so we want this in lowercase, unlike others
                std::string inventorySpecialStr = std::string(inventoryTexts[keywordSetupInventory[i]].get<std::string>());
                toLowercase(inventorySpecialStr);
                inventoryStringsVector.push_back(inventorySpecialStr);
            }
            else inventoryStringsVector.push_back(std::string(inventoryTexts[keywordSetupInventory[i]].get<std::string>()));
        }
        else
        {
            std::cerr << "JSON INVALID - -> " << fileNameForGameSetup
                      << ". A value in the \"INVENTORY TEXTS\" object has an invalid type or is missing." << std::endl;
            return false;
        }
    }

    // Now, we'll read four other values, for the printing animation and for the Game-over and continue screens.

    if (!root.contains(keywordSetupAnimationSpeed) ||
        !root.contains(keywordSetupLoad) ||
        !root.contains(keywordSetupSave) ||
        !root.contains(keywordSetupGameOverQuestionText))
    {
        std::cerr << "A key is missing in the JSON, the file is invalid -> " << fileNameForGameSetup << std::endl;
        std::cerr << "Make sure the file includes these keys: "
                  << keywordSetupAnimationSpeed << ", "
                  << keywordSetupLoad << ", "
                  << keywordSetupSave << ", "
                  << keywordSetupGameOverQuestionText << "." << std::endl;
        return false;
    }

    // Check the value types
    if (!root[keywordSetupAnimationSpeed].is_number_unsigned() ||
        !root[keywordSetupLoad].is_string() ||
        !root[keywordSetupSave].is_string() ||
        !root[keywordSetupGameOverQuestionText].is_string())
    {
        std::cerr << "JSON INVALID - -> " << fileNameForGameSetup << ". A key (Animation speed / Continue key / "
                     "Continue text / Game over text) has an invalid value." << std::endl;
        return false;
    }

    // Process values
    game.setAnimationSpeed(root[keywordSetupAnimationSpeed].get<int>());

    std::string saveStr = root[keywordSetupSave].get<std::string>();
    std::string loadStr = root[keywordSetupLoad].get<std::string>();

    toLowercase(saveStr);
    toLowercase(loadStr);

    game.texts.stringSave = saveStr;
    game.texts.stringLoad = loadStr;

    return true;
}


bool Reader::readMainCharacter()
{
    json root;
    std::string path = getPath(std::string(fileNameForMainCharacter));

    int jsonRead = readIntoJsonFromFile(path, root);
    if (jsonRead != 0)
    {
        if (jsonRead == ERROR_CANNOT_OPEN_FILE)
        {
            std::cerr << "CANNOT OPEN FILE " << fileNameForGameSetup << std::endl;
            return false;
        }
        else
        {
            std::cerr << "JSON INVALID -> " << fileNameForGameSetup << std::endl;
            return false;
        }
    }

    auto dummyStorage = Storage();

    return readEntities(std::string(fileNameForMainCharacter), dummyStorage, root, false, true);
}


bool Reader::readOneRoom(json& root, std::unique_ptr<Room> room)
{
    const json& description = root[keywordForRoomDescription];

    const json& nonTakableObjects = root[keywordForRoomNonTakableObjects];

    const json& takableObjects = root[keywordForRoomTakableObjects];

    const json& NPCs = root[keywordForRoomNPCs];

    if( !nonTakableObjects.is_object() || !takableObjects.is_object() || !NPCs.is_object() || !description.is_string() )
    {
        std::cerr   <<  "ROOMS INCLUDE AN INVALID JSON FILE"
                    <<  ". Make sure that the room's takable objects, non-takable objects and NPCs "
                        "each form a valid JSON object, and that the description is a string."<<std::endl;
        return false;
    }

    // reading non-takable objects
    if(!readEntities("ROOM JSON FILE (NON-TAKABLE OBJECTS)",
                     room->getRoomStorage(), nonTakableObjects, true, false))
        return false;

    // reading takable objects
    if(!readEntities("ROOM JSON FILE (TAKABLE OBJECTS)",
                     room->getRoomStorage(), takableObjects, false, false))
        return false;

    // reading the description
    room->setDescription( description.get<std::string>() );

    // reading NPCs
    if(!readEntities("ROOM JSON FILE (NPCs)",
                     room->getRoomStorage(), NPCs, false, false, true, room.get()))
        return false;

    game.addRoom(std::move(room));

    return true;
}


bool Reader::readEntities(const std::string& fileNameForErrors, Storage& storage, const json& objects,
                                bool staticObjects, bool readingMainCharacter, bool readingNPC, Room* roomForNCPs)
{

    // let's simplify the Action Responses map syntax
    using map_StringUnsigned_Tuple = std::map< std::pair<std::string, unsigned int>, std::tuple<std::string, std::string, std::vector<unsigned int>, int, int> >;

    //at this point &objects is guaranteed to be a JSON object

    for (json::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {

        // Validate "NAMES" array (which includes synonyms)
        if (!it.value().contains(keywordForNames))
        {
            std::cerr << "JSON INVALID -> " << fileNameForErrors
                      << ". An entity is missing the NAMES key-value pair." << std::endl;
            return false;
        }

        const json &namesArray = it.value()[keywordForNames];

        if (!namesArray.is_array()) {
            std::cerr << "JSON INVALID -> " << fileNameForErrors
                      << ". The value of 'NAMES' must be an array." << std::endl;
            return false;
        }

        for (const auto &name : namesArray) {
            if (!name.is_string()) {
                std::cerr << "JSON INVALID -> " << fileNameForErrors
                          << ". Each element of 'NAMES' must be a string." << std::endl;
                return false;
            }
        }

        auto names = std::make_unique< std::vector<std::string> >();

        for (const auto &name : namesArray) {
            std::string thisName = name.get<std::string>();

            toLowercase(thisName);

            names->push_back(thisName);
        }

        const std::string& nameID = (*names)[0];

        unsigned int quantity = 1;

        if(!it.value().contains(keywordForQuantity))
        {
            // if this is a static object or NPC, it's OK, Quantity is not needed
            if(!staticObjects && !readingNPC && !readingMainCharacter)
            {
                std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                            <<  ". A key-value pair is missing. Make sure each object contains "
                                "a key for its quantity."<<std::endl;
                return false;
            }
        }
        else if(!it.value()[keywordForQuantity].is_number_unsigned())
        {
            std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                        <<  ". The quantity value is of a wrong type. Make sure that it is a positive number."<<std::endl;
            return false;
        }
        else quantity = it.value()[keywordForQuantity].get<unsigned int>();

        if( !it.value().contains(keywordForStages) ||
            !it.value().contains(keywordForCommandResponses) )
        {
            std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                        <<  ". A key-value pair is missing. Make sure each object contains "
                            "a key for its stages and command responses."<<std::endl;
            return false;
        }

        if( !it.value()[keywordForStages].is_number_unsigned() ||
            !it.value()[keywordForCommandResponses].is_object() )
        {
            std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                        <<  ". A value is of a wrong type. Make sure that the number of stages "
                            "is a positive number and that the command responses form a valid JSON object."<<std::endl;
            return false;
        }

        unsigned int maxStage = (it.value()[keywordForStages].get<unsigned int>());

        if(maxStage == 0)
        {
            std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                        <<  ". The number of stages of an object must be greater than 0."<<std::endl;
            return false;
        }
        maxStage--;

        const json& commandResponses = it.value()[keywordForCommandResponses];

        std::unique_ptr<map_StringUnsigned_Tuple> actionResponses = std::make_unique<map_StringUnsigned_Tuple>();

        for (   json::const_iterator commandResponseIterator = commandResponses.begin();
                commandResponseIterator != commandResponses.end();
                ++commandResponseIterator )
        {
            std::string commandName = commandResponseIterator.key();
            toLowercase(commandName);

            unsigned int stage = 0;

            if(!commandResponseIterator.value().is_array())
            {
                std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                            <<  ". Make sure that the value of each command is an array."<<std::endl;
                return false;
            }

            for (const auto& commandResponse : commandResponseIterator.value())
            {

                if( !commandResponse.contains(keywordForResponseText) )
                {
                    std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                                <<  ". A command response object is missing a key-value pair "
                                    "for the text of the response."<<std::endl;
                    return false;
                }
                else if (!commandResponse[keywordForResponseText].is_string())
                {
                    std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                                <<  ". A command response's text is not a string."<<std::endl;
                    return false;
                }

                const std::string commandResponseText = commandResponse[keywordForResponseText].get<std::string>();

                int roomChangeValue = 0;

                if( commandResponse.contains(keywordForResponseRoomChange) )
                {
                    const json& roomChange = commandResponse[keywordForResponseRoomChange];

                    if (!roomChange.is_number_integer())
                    {
                        std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                                    <<  ". A command response's Room ID for Room Change is not an integer."
                                    <<std::endl;
                        return false;
                    }

                    roomChangeValue = roomChange.get<int>();
                }

                int stagesUpdateEventValue = 0;

                if( commandResponse.contains(keywordForResponseStagesUpdate) )
                {
                    const json& stagesUpdate = commandResponse[keywordForResponseStagesUpdate];

                    if (!stagesUpdate.is_number_unsigned())
                    {
                        std::cerr   <<  "JSON INVALID -> "<<fileNameForErrors
                                    <<  ". A command response's 'Stages Update Event ID' is not an unsigned integer."<<std::endl;
                        return false;
                    }

                    stagesUpdateEventValue = stagesUpdate.get<int>();
                }

                //finally load and check the attribute requirements and if they're present, also the alt. text

                std::vector<unsigned int> requirementsVector; // Default value for requirementsVector
                std::string commandResponseAltText; // Default value for commandResponseAltText = ("")

                if (commandResponse.contains(keywordForResponseRequirements))
                {
                    const json& requirements = commandResponse[keywordForResponseRequirements];

                    if (!requirements.is_array())
                    {
                        std::cerr << "JSON INVALID -> " << fileNameForErrors << ". The attribute requirements value"
                                                                                "in command response must be an array of integers." << std::endl;
                        return false;
                    }

                    for (const auto& requirement : requirements)
                    {
                        if (!requirement.is_number_unsigned())
                        {
                            std::cerr << "JSON INVALID -> " << fileNameForErrors
                                      << ". One of the requirements' values in command response"
                                         " is not an integer." << std::endl;
                            return false;
                        }

                        requirementsVector.push_back(requirement.get<unsigned int>());
                    }

                    if( requirementsVector.size() != game.texts.attributeNames.size())
                    {
                        std::cerr << "JSON INVALID -> " << fileNameForErrors
                                  << ". The number of attribute requirements does not match the number of the game's"
                                     " attributes." << std::endl;
                        return false;
                    }

                    if (commandResponse.contains(keywordForResponseAltText))
                    {
                        const json& altText = commandResponse[keywordForResponseAltText];

                        if (!altText.is_string())
                        {
                            std::cerr << "JSON INVALID -> " << fileNameForErrors
                                      << ". The alt text value in command response is not a string." << std::endl;
                            return false;
                        }

                        commandResponseAltText = altText.get<std::string>();
                    }
                    else {
                        std::cerr << "JSON INVALID -> " << fileNameForErrors
                                  << ". Command response includes requirements, but not ALT TEXT." << std::endl;
                        return false;
                    }
                }

                (*actionResponses)[ {commandName, stage} ] = {      commandResponseText,
                                                                    commandResponseAltText,
                                                                    requirementsVector,
                                                                    roomChangeValue,
                                                                    stagesUpdateEventValue };

                stage++;
            }
        }

        std::unique_ptr<CombatStats> combatStats = nullptr;
        bool combatStatsOK = true;

        // attempt to read Combat Stats

        if (it.value().contains(keywordForCombatStats))
        {
            const json& combatStatsJson = it.value()[keywordForCombatStats];
            if (!combatStatsJson.is_array() || combatStatsJson.size() != COMBAT_STATS_ARRAY_SIZE)
            {
                combatStatsOK = false;
            }
            else
            {
                for (const auto& value : combatStatsJson)
                {
                    if (!value.is_number_unsigned())
                    {
                        combatStatsOK = false;
                        break;
                    }
                }

                if (combatStatsOK)
                {
                    int remaining = combatStatsJson[0].get<int>();
                    int power = combatStatsJson[1].get<int>();
                    int speed = combatStatsJson[2].get<int>();

                    combatStats = std::make_unique<CombatStats>(remaining, power, speed);
                }
            }
        }

        if (!combatStatsOK)
        {
            std::cerr << "JSON INVALID -> " << fileNameForErrors
                      << ". File contains invalid Combat Stats." << std::endl;

            return false;
        }

        bool addedNamesToMap = false;

        if(!readingNPC && !readingMainCharacter)
        {
            auto thisObject = std::make_shared< GameObject > (nameID , maxStage, !staticObjects, std::move(actionResponses), std::move(combatStats));
            storage.add(thisObject, quantity);
            addedNamesToMap = game.addNamesToEntityMap(*names, thisObject);
        }
        else if(readingNPC)
        {
            unsigned int health = DEFAULT_NPC_HEALTH;

            if(combatStats) // if NPC is fight-able
            {
                // since NPC is fightable, the object must also include the key for Health

                bool healthOK = true;

                if(!it.value().contains(keywordForNPCsHealth))
                    healthOK = false;
                else if(!it.value()[keywordForNPCsHealth].is_number_unsigned())
                    healthOK = false;

                if(!healthOK)
                {
                    std::cerr   <<  "NPC LOGIC ERROR IN THIS FILE -> "<<fileNameForErrors<<" ! Each fightable NPC must "
                                                                                           "have the 'health' key specified, and it must be a positive integer."<<std::endl;
                    return false;
                }

                health = it.value()[keywordForNPCsHealth].get<unsigned int>();
            }

            auto thisNPC = std::make_shared<NPC>(nameID, maxStage, std::move(actionResponses), health, std::move(combatStats));

            if(!roomForNCPs->getRoomStorage().add(thisNPC))
            {
                std::cerr   <<  "NPC LOGIC ERROR IN THIS FILE -> "<<fileNameForErrors<<" !"<<std::endl;
                return false;
            }

            addedNamesToMap = game.addNamesToEntityMap(*names, thisNPC);
        }

        else if(readingMainCharacter)
        {

            auto mainCharacter = std::make_shared<MainCharacter> (  nameID , maxStage, game.getInventory(), std::move(actionResponses));

            addedNamesToMap = game.addNamesToEntityMap(*names, mainCharacter);

            auto& attributesVec = mainCharacter->getAttributes();

            for( const auto& attributeName : game.texts.attributeNames )
            {
                // create a new attribute of the main character
                auto attribute = std::make_unique<Attribute>(attributeName);
                attributesVec.push_back(std::move(attribute));
            }

            game.setMainCharacter(std::move(mainCharacter));
        }

        if(!addedNamesToMap)
            return false;
    }

    return true;
}



