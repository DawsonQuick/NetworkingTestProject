#pragma once
#ifndef GLOBALCONFIGURATION_H
#define GLOBALCONFIGURATION_H
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <typeindex>
#include "./../../OpenGL/Utils/Texture.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"
#include "./TileManager/TileManager.h"

enum MeasurmentSystem {
	GOEMETRIC,
	GRID,
};




class GlobalConfigurations {
private:
	float Scale;




	glm::vec4 GridColor;
	glm::vec4 CursorColor;

	std::string selectedAction;
	std::shared_ptr<Spell> selectedSpell;
	bool spellChangedState; 

	MeasurmentSystem currentSystem;

	float currentSpellShotCount;


	GlobalConfigurations() {
		Scale = 26.0;
		currentSystem = MeasurmentSystem::GRID;
		spellChangedState = false;
	}

	// Delete copy constructor and assignment operator
	GlobalConfigurations(const GlobalConfigurations&) = delete;
	GlobalConfigurations& operator=(const GlobalConfigurations&) = delete;



public:
	// Singleton Constructor
	static GlobalConfigurations& getInstance() {
		static GlobalConfigurations instance;
		return instance;
	}

	float getScale() {
		return Scale;
	}
	
	void setScale(float tmpScale) {
		Scale = tmpScale;
	}

	void setMapTileInformation(std::vector<TileInfo> tileInformation) {
		TileManager::getInstance().setMapTileInformation(tileInformation);
	}

	std::vector<TileInfo> getMapTileInformation() {
		return TileManager::getInstance().getMapTileInformation();
	}

	void setCurrentlyHoveredTile(TileInfo tile) {
		TileManager::getInstance().setCurrentlyHoveredTile(tile);
	}

	TileInfo getCurrentlyHoveredTile() {
		return TileManager::getInstance().getCurrentlyHoveredTile();
	}



	void setCurrentlPlayerTile(TileInfo tile) {
		TileManager::getInstance().setCurrentlPlayerTile(tile);
	}

	TileInfo getCurrentlPlayerTile() {
		return TileManager::getInstance().getCurrentlPlayerTile();
	}

	void setGridColor(glm::vec4 gridColor) {
		GridColor = gridColor;
	}

	glm::vec4 getGridColor() {
		return GridColor;
	}
	void setCursorColor(glm::vec4 cursorColor) {
		CursorColor = cursorColor;
	}

	glm::vec4 getCursorColor() {
		return CursorColor;
	}

	void setselectedAction(std::string action) {
		selectedAction = action;
	}
	std::string getselectedAction() {
		return selectedAction;
	}

	void setSelectedSpell(std::shared_ptr<Spell> spell) {
		currentSpellShotCount = 1.0f; //reset shot count for change in spell
		spellChangedState = true;
		selectedSpell = spell;
	}
	std::shared_ptr<Spell> getSelectedSpell() {
		return selectedSpell;
	}
	MeasurmentSystem getCurrentMeasurmentSystem() {
		return currentSystem;
	}

	void setCurrentMeasurementSystem(MeasurmentSystem tmpSystem) {
		currentSystem = tmpSystem;
	}

	void setCurrentSpellShotCount(float count) {
		currentSpellShotCount = count;
	}
	float getCurrentSpellShotCount() {
		return currentSpellShotCount;
	}

	void resetChangeState(bool changeState) {
		spellChangedState = changeState;
	}

	bool hasSpellChanged(){
		return spellChangedState;
	}
	void setTileDimemsions(int width, int height) {
		TileManager::getInstance().setTileDimemsions(width,height);
	}

	//Temp void
	std::vector<float> getA_Starpath() {
		return std::move(TileManager::getInstance().getA_Starpath());
	}



	





};

#endif
