//
// Created by Mario on 26/05/2023.
//

#pragma once

#ifndef SRC_READER_H
#define SRC_READER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "../../libs/json.hpp"
#include "../Game.h"

constexpr std::string_view fileNameForTypesAndAttributes      = "types-and-attributes.json";
constexpr std::string_view fileNameForCommands                = "commands.json";

constexpr std::string_view fileNameForGameSetup               = "game-setup.json";
constexpr std::string_view fileNameForInventoryStorage        = "main-character-storage.json";

constexpr std::string_view fileNameForMainCharacter           = "main-character.json";

constexpr std::string_view fileNameForExtStorageNoExtension   = "external-storage-";
constexpr std::string_view fileNameForRoomNoExtension         = "room-";

constexpr std::string_view fileNameForStagesUpdates           = "stages-updates.json";
constexpr std::string_view fileNameForCombat                  = "combat.json";

constexpr std::string_view directoryNameForRooms              = "rooms";
constexpr std::string_view directoryNameForInventory          = "inventory";
constexpr std::string_view directoryNameForEvents             = "events";

constexpr std::string_view keywordForCommands                 = "COMMANDS";
constexpr std::string_view keywordForCommandForAdding         = "COMMAND FOR ADDING TO STORAGE/INVENTORY";
constexpr std::string_view keywordForCommandForRemoving       = "COMMAND FOR REMOVING FROM STORAGE/INVENTORY";
constexpr std::string_view keywordForCommandForCombat         = "COMMAND FOR STARTING COMBAT";
constexpr std::string_view keywordForCommandForEquip          = "COMMAND FOR EQUIPPING TO HAND";
constexpr std::string_view keywordForCommandForUnequip        = "COMMAND FOR UN-EQUIPPING FROM HAND";

constexpr std::string_view keywordForAttributes               = "ATTRIBUTES";
constexpr std::string_view keywordForCharacterTypes           = "CHARACTER TYPES";
constexpr std::string_view keywordForN                        = "PLAYER'S MAX INFLUENCE";

constexpr std::string_view keywordForNames                    = "NAMES";
constexpr std::string_view keywordForCapacity                 = "MAX CAPACITY";
constexpr std::string_view keywordForObjects                  = "OBJECTS";
constexpr std::string_view keywordForQuantity                 = "QUANTITY";
constexpr std::string_view keywordForStages                   = "STAGES";

constexpr std::string_view keywordForCombatStats              = "COMBAT STATS [USES REMAINING, HIT POWER, HIT SPEED]";
static const int COMBAT_STATS_ARRAY_SIZE                      = 3;

constexpr std::string_view keywordForCommandResponses         = "COMMAND RESPONSES";
constexpr std::string_view keywordForResponseText             = "TEXT";
constexpr std::string_view keywordForResponseAltText          = "ALT TEXT";
constexpr std::string_view keywordForResponseRoomChange       = "CHANGE TO ROOM";
constexpr std::string_view keywordForResponseStagesUpdate     = "ID OF STAGES UPDATE EVENT";
constexpr std::string_view keywordForResponseRequirements     = "ATTRIBUTE REQUIREMENTS";

constexpr std::string_view keywordForNPCsHealth               = "HEALTH";

constexpr std::string_view keywordForObjectInRightHand        = "OBJECT IN RIGHT HAND";
constexpr std::string_view keywordForObjectInLeftHand         = "OBJECT IN LEFT HAND";

constexpr std::string_view keywordForNumberOfExternalStorages = "NUMBER OF EXTERNAL STORAGES TO BE LOADED INTO INVENTORY";
constexpr std::string_view keywordForNumberOfRooms            = "NUMBER OF ROOMS";

constexpr std::string_view keywordForRoomTakableObjects       = "TAKABLE OBJECTS";
constexpr std::string_view keywordForRoomNonTakableObjects    = "NON-TAKABLE OBJECTS";
constexpr std::string_view keywordForRoomNPCs                 = "NPCS";
constexpr std::string_view keywordForRoomDescription          = "ROOM DESCRIPTION (SHOWN WHEN ENTERED, BUT CAN BE SHOWN ON COMMAND LATER, TOO)";

constexpr std::string_view keywordForStagesUpdatesMainArray   = "STAGES-UPDATES EVENTS";
constexpr std::string_view keywordForStagesUpdatesID          = "STAGES-UPDATES EVENT ID";
constexpr std::string_view keywordForStagesUpdatesPairs       = "ENTITY - STAGE PAIRS";

constexpr std::string_view keywordSetupAnimationSpeed         = "PRINT ANIMATION SPEED FOR ONE CHARACTER IN MS (DEFAULT = 12)";
constexpr std::string_view keywordSetupLoad                   = "WORD FOR LOADING";
constexpr std::string_view keywordSetupSave                   = "WORD FOR SAVING";
constexpr std::string_view keywordSetupGameOverQuestionText   = "TEXT - GAME OVER, DO YOU WANT TO PLAY AGAIN?";

constexpr std::string_view keywordSetupIntroTexts             = "INTRO TEXTS";
constexpr std::string_view keywordSetupInventoryTexts         = "INVENTORY TEXTS";
constexpr std::string_view keywordSetupWrongInputText         = "WRONG INPUT TEXT";
constexpr std::string_view keywordSetupGameOverText           = "GAME OVER TEXT (Also ask if player wants to play again)";
constexpr std::string_view keywordSetupWordForYes             = "WORD FOR YES";
constexpr std::string_view keywordSetupWordForNo              = "WORD FOR NO";
constexpr std::string_view keywordSetupStringsToBeIgnored     = "STRINGS TO BE IGNORED";
constexpr std::string_view keywordSetupNamesForRoom           = "NAMES FOR A ROOM (IF THE PLAYERS ENTERS THESE WORDS, THIS GAME WILL REFER TO THE CURRENT ROOM)";

constexpr std::string_view keywordSetupIntro1                 = "First intro text, e.g. "
                                                                "'Welcome to the world of Adventure Game XY. "
                                                                "Now let's create your character!' ";

constexpr std::string_view keywordSetupIntro2                 = "Post-types-print text, e.g. "
                                                                "'These are the available Character Types. "
                                                                "Choose one now by entering its number.";

constexpr std::string_view keywordSetupIntro3                 = "Attributes intro text, "
                                                              "e.g. 'Great, now, we give you [PLAYER'S MAX INFLUENCE]"
                                                              " coins that you can spend on improving your character. "
                                                              "There are 4 attributes - Health, Power, IQ and Luck. "
                                                              "Tell us how much you want to spend on each.";

constexpr std::string_view keywordSetupIntro4                 = "Post character creation text, e.g. 'All set. Let's play.";

constexpr std::string_view keywordSetupInventory[6] =
{
        "String for displaying inventory",
        "Text like '$ITEM_NAME was added to your inventory' (after $ITEM_NAME)",
        "Text like '$ITEM_NAME was removed from your inventory' (after $ITEM_NAME)",
        "Text like 'Cannot add to inventory because it's full.'",
        "Text like 'Equipping/Unequipping successul!'",
        "Text like 'Equipping/Unequipping NOT successul!'"
};

constexpr std::string_view keywordCombatText[9] =
{
        "A string similar to 'You fought with' which will be followed by NPC's name",
        "A string similar to 'using' which will be followed by the main character's weapon name",
        "A string similar to 'Your health is at' which will be followed by the health percentage",
        "A string similar to 'The enemy finished you, you're dead'",
        "Part 1 - A string similar to 'The health of (ENEMY->name) is at (HEALTH->percentage)%.' The part before (ENEMY->name)",
        "Part 2 - A string similar to 'The health of (ENEMY->name) is at (HEALTH->percentage)%.' The part between (ENEMY->name) & (HEALTH->percentage)",
        "Part 1 - A string similar to 'You defeated (ENEMY->name), it's dead.' The part before (ENEMY->name)",
        "Part 2 - A string similar to 'You defeated (ENEMY->name), it's dead.' The part after (ENEMY->name)",
        "A string similar to 'You both survived. You can now attack again.' "
};

static const int ERROR_CANNOT_OPEN_FILE   = 100;
static const int ERROR_JSON_INVALID       = 200;


const char separator =
#ifdef _WIN32
        '\\';
#else
        '/';
#endif

/**
 * @class Reader
 * Responsible for reading the configuration files in JSON format and loading this data into a Game object.
 */
class Reader
{
    public:

        Reader(std::string directory, Game& game);

        /**
         * Read the configuration from the specified JSON config file.
         */
        bool readEverything();

    private:

        Game& game;

        const std::string directory;

        static int readIntoJsonFromFile(const std::string& fileName, nlohmann::json& root);

        // The following methods read the parts of the game into the Game instance.

        bool readCommands();

        bool readTypesAndAttributes();

        bool readRooms();

        bool readMainCharacter();

        bool readInventoryStorage();

        bool readStagesUpdates();

        bool readOneStagesUpdatesObject(const nlohmann::json& object);

        bool readCombat();

        bool readEssentialTextsAndValues();

        bool readOneRoom(nlohmann::json& root, std::unique_ptr<Room> room);

        bool readEntities(const std::string& fileName, Storage& storage, const nlohmann::json& objects,
                          bool staticObjects, bool readingMainCharacter,
                          bool readingNPC = false, Room* roomForNPCs = nullptr);

        std::string getPath(const std::string& fileName, const std::string& subDirectory = "");

        std::string getPathForRoom(unsigned int number);

        /** Transform any string to lowercase */
        void toLowercase(std::string &str);
};

#endif //SRC_READER_H
