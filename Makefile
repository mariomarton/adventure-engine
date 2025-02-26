CXX = g++
CXX_FLAGS = -std=c++17 -O2 -fsanitize=undefined -Wall -pedantic

TARGET = run-adventure-engine

# System commands
MKDIR ?= mkdir
RM ?= rm
RM_FLAGS ?= -r # We want to delete everything in the build directory

# Project directories
SRC_DIR ?= src
EVENTS_DIR ?= src/Events
INVENTORY_DIR ?= src/Inventory
GAMEENTITY_DIR ?= src/GameEntity
TYPEANDATTRIBUTE_DIR ?= src/TypeAndAttribute
UTIL_DIR ?= src/Util

# Directories that will be deleted by make clean
DOC_DIR ?= doc
BUILD_DIR ?= .bin

HEADER_FILES = \
	$(SRC_DIR)/Game.h \
	$(SRC_DIR)/Room.h \
	$(EVENTS_DIR)/StagesUpdatesEvent.h \
	$(GAMEENTITY_DIR)/CombatStats.h \
    $(GAMEENTITY_DIR)/GameEntity.h \
    $(GAMEENTITY_DIR)/GameObject.h \
    $(GAMEENTITY_DIR)/MainCharacter.h \
    $(GAMEENTITY_DIR)/NPC.h \
    $(INVENTORY_DIR)/InventoryStorage.h \
    $(INVENTORY_DIR)/Storage.h \
    $(TYPEANDATTRIBUTE_DIR)/Attribute.h \
    $(TYPEANDATTRIBUTE_DIR)/MainCharacterType.h \
    $(UTIL_DIR)/Texts.h \
    $(UTIL_DIR)/Reader.h \
    $(UTIL_DIR)/IO.h

OBJECT_FILES = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/Reader.o \
	$(BUILD_DIR)/Game.o \
	$(BUILD_DIR)/Room.o \
    $(BUILD_DIR)/IO.o \
	$(BUILD_DIR)/StagesUpdatesEvent.o \
	$(BUILD_DIR)/GameEntity.o \
	$(BUILD_DIR)/GameObject.o \
	$(BUILD_DIR)/MainCharacter.o \
	$(BUILD_DIR)/NPC.o \
	$(BUILD_DIR)/InventoryStorage.o \
	$(BUILD_DIR)/Storage.o \
	$(BUILD_DIR)/Attribute.o \
	$(BUILD_DIR)/MainCharacterType.o


.PHONY: all
all: compile doc

.PHONY: compile
compile: $(TARGET)

.PHONY: run
run:
	@read -p "What game do you want to play?: " argument; \
	./$(TARGET) $$argument


.PHONY: clean
clean:
	$(RM) $(RM_FLAGS) $(TARGET) $(BUILD_DIR) $(DOC_DIR)

.PHONY: doc
doc: $(HEADER_FILES)
	doxygen Doxyfile 2> /dev/null

# Rules

# Create the Build directory
$(BUILD_DIR):
	$(MKDIR) $(MKDIR_FLAGS) $(BUILD_DIR)

# 'adventureengine' executable
$(TARGET): $(OBJECT_FILES) | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $^ -o $@


# Modules (for each directory)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(EVENTS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(GAMEENTITY_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(INVENTORY_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(TYPEANDATTRIBUTE_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(UTIL_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

# Dependencies
# using this -> find src \( -path src/cmake-build-debug -o -path src/.idea \) -prune -o -type f -name "*.cpp" -exec g++ -std=c++17 -MM {} \; > dependencies.mk

GameEntity.o: src/GameEntity/GameEntity.cpp src/GameEntity/GameEntity.h \
 src/GameEntity/CombatStats.h
MainCharacter.o: src/GameEntity/MainCharacter.cpp \
 src/GameEntity/MainCharacter.h src/GameEntity/GameEntity.h \
 src/GameEntity/CombatStats.h \
 src/GameEntity/../Inventory/InventoryStorage.h \
 src/GameEntity/../Inventory/Storage.h \
 src/GameEntity/../Inventory/../GameEntity/GameObject.h \
 src/GameEntity/../Inventory/../GameEntity/GameEntity.h \
 src/GameEntity/../TypeAndAttribute/MainCharacterType.h \
 src/GameEntity/../TypeAndAttribute/Attribute.h
GameObject.o: src/GameEntity/GameObject.cpp src/GameEntity/GameObject.h \
 src/GameEntity/GameEntity.h src/GameEntity/CombatStats.h
NPC.o: src/GameEntity/NPC.cpp src/GameEntity/NPC.h \
 src/GameEntity/GameEntity.h src/GameEntity/CombatStats.h \
 src/GameEntity/../TypeAndAttribute/Attribute.h
IO.o: src/Util/IO.cpp src/Util/IO.h
MainCharacterType.o: src/TypeAndAttribute/MainCharacterType.cpp \
 src/TypeAndAttribute/MainCharacterType.h \
 src/TypeAndAttribute/Attribute.h
Attribute.o: src/TypeAndAttribute/Attribute.cpp \
 src/TypeAndAttribute/Attribute.h
Room.o: src/Room.cpp src/Room.h src/GameEntity/GameEntity.h \
 src/GameEntity/CombatStats.h src/Inventory/Storage.h \
 src/Inventory/../GameEntity/GameObject.h \
 src/Inventory/../GameEntity/GameEntity.h src/GameEntity/NPC.h \
 src/GameEntity/GameEntity.h \
 src/GameEntity/../TypeAndAttribute/Attribute.h
InventoryStorage.o: src/Inventory/InventoryStorage.cpp \
 src/Inventory/InventoryStorage.h src/Inventory/Storage.h \
 src/Inventory/../GameEntity/GameObject.h \
 src/Inventory/../GameEntity/GameEntity.h \
 src/Inventory/../GameEntity/CombatStats.h
Storage.o: src/Inventory/Storage.cpp src/Inventory/Storage.h \
 src/Inventory/../GameEntity/GameObject.h \
 src/Inventory/../GameEntity/GameEntity.h \
 src/Inventory/../GameEntity/CombatStats.h
StagesUpdatesEvent.o: src/Events/StagesUpdatesEvent.cpp \
 src/Events/StagesUpdatesEvent.h src/Events/../GameEntity/GameEntity.h \
 src/Events/../GameEntity/CombatStats.h \
 src/Events/../GameEntity/GameObject.h \
 src/Events/../GameEntity/GameEntity.h
main.o: src/main.cpp src/Util/Reader.h src/Util/../../libs/json.hpp \
 src/Util/../Game.h src/Util/../Room.h \
 src/Util/../GameEntity/GameEntity.h src/Util/../GameEntity/CombatStats.h \
 src/Util/../Inventory/Storage.h \
 src/Util/../Inventory/../GameEntity/GameObject.h \
 src/Util/../Inventory/../GameEntity/GameEntity.h \
 src/Util/../GameEntity/NPC.h src/Util/../GameEntity/GameEntity.h \
 src/Util/../GameEntity/../TypeAndAttribute/Attribute.h \
 src/Util/../GameEntity/MainCharacter.h \
 src/Util/../GameEntity/../Inventory/InventoryStorage.h \
 src/Util/../GameEntity/../Inventory/Storage.h \
 src/Util/../GameEntity/../TypeAndAttribute/MainCharacterType.h \
 src/Util/../GameEntity/../TypeAndAttribute/Attribute.h \
 src/Util/../Events/StagesUpdatesEvent.h \
 src/Util/../Events/../GameEntity/GameEntity.h \
 src/Util/../Events/../GameEntity/GameObject.h src/Util/../Util/Texts.h
Reader.o: src/Util/Reader.cpp src/Util/Reader.h \
 src/Util/../../libs/json.hpp src/Util/../Game.h src/Util/../Room.h \
 src/Util/../GameEntity/GameEntity.h src/Util/../GameEntity/CombatStats.h \
 src/Util/../Inventory/Storage.h \
 src/Util/../Inventory/../GameEntity/GameObject.h \
 src/Util/../Inventory/../GameEntity/GameEntity.h \
 src/Util/../GameEntity/NPC.h src/Util/../GameEntity/GameEntity.h \
 src/Util/../GameEntity/../TypeAndAttribute/Attribute.h \
 src/Util/../GameEntity/MainCharacter.h \
 src/Util/../GameEntity/../Inventory/InventoryStorage.h \
 src/Util/../GameEntity/../Inventory/Storage.h \
 src/Util/../GameEntity/../TypeAndAttribute/MainCharacterType.h \
 src/Util/../GameEntity/../TypeAndAttribute/Attribute.h \
 src/Util/../Events/StagesUpdatesEvent.h \
 src/Util/../Events/../GameEntity/GameEntity.h \
 src/Util/../Events/../GameEntity/GameObject.h src/Util/../Util/Texts.h \
 src/Util/../GameEntity/NPC.h src/Util/../Inventory/InventoryStorage.h \
 src/Util/../Events/StagesUpdatesEvent.h \
 src/Util/../TypeAndAttribute/Attribute.h
Game.o: src/Game.cpp src/Game.h src/Room.h src/GameEntity/GameEntity.h \
 src/GameEntity/CombatStats.h src/Inventory/Storage.h \
 src/Inventory/../GameEntity/GameObject.h \
 src/Inventory/../GameEntity/GameEntity.h src/GameEntity/NPC.h \
 src/GameEntity/GameEntity.h \
 src/GameEntity/../TypeAndAttribute/Attribute.h \
 src/GameEntity/MainCharacter.h \
 src/GameEntity/../Inventory/InventoryStorage.h \
 src/GameEntity/../Inventory/Storage.h \
 src/GameEntity/../TypeAndAttribute/MainCharacterType.h \
 src/GameEntity/../TypeAndAttribute/Attribute.h \
 src/Events/StagesUpdatesEvent.h src/Events/../GameEntity/GameEntity.h \
 src/Events/../GameEntity/GameObject.h src/Util/Texts.h src/Util/IO.h \
 src/../libs/json.hpp
