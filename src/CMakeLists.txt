cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-long-long -O0 -g -ggdb")


#include_directories(${CMAKE_SOURCE_DIR}/../libs)  # Add the path to "libs" directory

add_executable(src
        main.cpp
        GameEntity/GameEntity.h
        GameEntity/NPC.h
        GameEntity/MainCharacter.h
        TypeAndAttribute/MainCharacterType.h
        GameEntity/GameObject.h
        Room.h
        Game.h
        Inventory/Storage.h
        Inventory/InventoryStorage.h
        TypeAndAttribute/Attribute.h
        Inventory/Storage.cpp
        Inventory/InventoryStorage.cpp
        TypeAndAttribute/Attribute.cpp
        TypeAndAttribute/MainCharacterType.cpp
        GameEntity/GameEntity.cpp
        GameEntity/GameObject.cpp
        GameEntity/MainCharacter.cpp
        GameEntity/NPC.cpp
        Util/Reader.h
        Util/Reader.cpp
        Game.cpp Room.cpp main.cpp GameEntity/NPC.cpp Events/StagesUpdatesEvent.h Events/StagesUpdatesEvent.cpp GameEntity/CombatStats.h Util/IO.h Util/IO.cpp Util/Texts.h Tests/tests.cpp Tests/tests.h)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

#target_link_libraries(src ${CMAKE_SOURCE_DIR}/../libs/json.hpp)  # Link the "json.hpp" library

