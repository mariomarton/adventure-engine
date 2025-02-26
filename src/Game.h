//
// Created by Mario on 12/05/2023.
//

#pragma once

#ifndef SRC_GAME_H
#define SRC_GAME_H

#include <vector>
#include <unordered_map>
#include "Room.h"
#include "GameEntity/MainCharacter.h"
#include "Events/StagesUpdatesEvent.h"
#include "Util/Texts.h"

/**
 * @class Game
 * Handles running the game loop, running combat, saving and loading, and more.
 * */
class Game
{
        public:

                /** @param path - path to the directory with the game */
                Game(std::string path);

                Game(const Game& other) = default;
                ~Game() = default;

                /** Launches the game-loop.
                 * @throws std::runtime_error if unable to launch. */
                void startGame();

                /** Various Game-Texts read from config files. */
                Texts texts;

                // The following public methods are just getters and setters.

                std::unordered_map<std::string, std::vector<std::string>>& getCommandsMap();

                std::vector< std::shared_ptr<MainCharacterType> >& getMainCharacterTypes();

                InventoryStorage& getInventory();

                void addRoom(std::unique_ptr<Room> room);

                void addStagesUpdatesEvent(std::unique_ptr<StagesUpdatesEvent> event);

                bool addNamesToEntityMap(const std::vector<std::string>& names, std::shared_ptr<GameEntity> entity);

                void setNumberOfRooms(unsigned int n);

                void setMainCharacter(std::shared_ptr<MainCharacter> character);

                void setPlayerMaxInfluence(unsigned int maxInfluence);

                void setAnimationSpeed(int speed);

        private:

                std::string pathToGame;

                std::shared_ptr<MainCharacter> mainCharacter;
                InventoryStorage inventory;

                bool isRunning;

                // A key map for finding the entity the player entered
                std::unordered_map< std::string, std::shared_ptr<GameEntity> > entities;

                // Map to store commands and their synonyms
                std::unordered_map<std::string, std::vector<std::string>> commands;

                // Vector of available Character Types
                std::vector< std::shared_ptr<MainCharacterType> > mainCharacterTypes;

                // Vector of StagesUpdatesEvents
                std::vector< std::unique_ptr<StagesUpdatesEvent> > stagesUpdatesEvents;

                // Vector of all rooms
                std::vector<std::unique_ptr<Room>> rooms;

                unsigned int currentRoomIndex;
                unsigned int numberOfRooms;

                int printAnimationSpeed;

                unsigned int playerMaxInfluence;

                void runIntro();

                /** Handles the game-loop */
                void runGame();

                /** Handles game-loop within one Room */
                void runRoom();

                /** Saves the game into a save-file */
                void saveGame();

                /** Loads the game from a save-file */
                void loadGame();

                /** Custom method for printing to console ("animated text") */
                void print(const std::string& text);

                /**
                 * Attempts to find the command from the player's input.
                 * Saves it's non-synonym value to strForCommand.
                 * */
                bool findCommand(const std::string &verbFromInput, std::string &strForCommand) const;

                /** Attempts to find the entity from player's input. */
                [[nodiscard]] std::shared_ptr<GameEntity> findEntity(const std::string &noun) const;

                /** Launches StagesUpdates. */
                void triggerStagesUpdates(StagesUpdatesEvent& event);

                /** Handles Combat logic. */
                bool launchCombat(std::shared_ptr<GameEntity> npc);

                /** Removes the entity. E.g. after NPC dies. */
                void removeEntityFromMap(GameEntity& entityToBeRemoved);

                /**
                 * Gets the Verb and the Noun from player's input
                 * Handles lower/upper-case and special symbols. Ignores "stringsToBeIgnored."
                 * */
                void getVerbAndNounFromInput(std::string& verb, std::string& noun);

                /** Loads a number between min and max into the variableForInput parameter. */
                void getSingleNumberInput(unsigned int& variableForInput, unsigned int min, unsigned int max);

                /** Determines whether word is reffering to a Room. */
                bool wordRefersToRoom(const std::string& word);

                /** Determines whether word from input should be ignored. */
                bool ignoreThisWord(const std::string &word);
};

#endif //SRC_GAME_H
