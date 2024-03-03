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

struct TileInfo{
	glm::mat4x3 vertexPos;
	glm::vec2 centerPos;
	glm::vec2 layoutIndex;
	bool isTraversable;

};

enum MeasurmentSystem {
	GOEMETRIC,
	GRID,
};




class GlobalConfigurations {
private:
	float Scale;
	std::vector<TileInfo> MapTileInformation;
	TileInfo currentlyHoveredTile;

	glm::vec4 GridColor;
	glm::vec4 CursorColor;

	std::string selectedAction;
	std::shared_ptr<Spell> selectedSpell;

	MeasurmentSystem currentSystem;

	GlobalConfigurations() {
		Scale = 20.0;
		currentSystem = MeasurmentSystem::GRID;
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
		MapTileInformation = tileInformation;
	}

	std::vector<TileInfo> getMapTileInformation() {
		return MapTileInformation;
	}

	void setCurrentlyHoveredTile(TileInfo tile) {
		currentlyHoveredTile = tile;
	}

	TileInfo getCurrentlyHoveredTile() {
		return currentlyHoveredTile;
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
	


	





};

#endif
