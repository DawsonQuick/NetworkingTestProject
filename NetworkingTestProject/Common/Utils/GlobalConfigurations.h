#pragma once
#ifndef GLOBALCONFIGURATION_H
#define GLOBALCONFIGURATION_H
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <typeindex>
#include <functional>
#include <sstream>
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
	float timePerTurn; // In ms
	glm::vec4 GridColor;
	glm::vec4 CursorColor;
	std::string selectedAction;
	MeasurmentSystem currentSystem;
	std::string playerName;
	std::function<void(const char* msg)> callback;
	bool gridPaintMode;

	GlobalConfigurations() {
		Scale = 25.0;
		timePerTurn = 6000; //Each Turn will take 6 seconds
		TileManager::getInstance().setSideLength(Scale);
		currentSystem = MeasurmentSystem::GRID;
		gridPaintMode = false;
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
	void setPlayerName(std::string tmpPlayerName) {
		playerName = tmpPlayerName;
	}
	std::string getPlayerName() {
		return playerName;
	}

	void setClientSendMessageCallback(std::function<void(const char* msg)> Tmpcallback) {
		callback = Tmpcallback;
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
	std::vector<glm::mat2x2> getBlockedTileWalls() {
		return TileManager::getInstance().getBlockedTileWalls();
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


	MeasurmentSystem getCurrentMeasurmentSystem() {
		return currentSystem;
	}

	void setCurrentMeasurementSystem(MeasurmentSystem tmpSystem) {
		currentSystem = tmpSystem;
	}



	void updateTileMapInformation(float indexX, float indexY) {
		TileManager::getInstance().updateTileMapInformation(indexX, indexY);
	}

	void setgridPaintMode(bool isPainting) {
		gridPaintMode = isPainting;
	}
	bool getgridPaintMode() {
		return gridPaintMode;
	}

	std::vector<float> getBlockedTilesVertexArray() {
		return TileManager::getInstance().getBlockedTilesVertexArray();
	}
	std::vector<unsigned int> getBlockedTileIndiceArray() {
		return TileManager::getInstance().getBlockedTileIndiceArray();
	}

	void setTileDimemsions(int width, int height) {
		TileManager::getInstance().setTileDimemsions(width,height);
	}

	//Temp void
	std::vector<float> getA_Starpath() {
		return std::move(TileManager::getInstance().getA_Starpath());
	}

	void sendMsg(const char* msg) {
		callback(msg);
	}


};

#endif
