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





class GlobalConfigurations {
private:
	float Scale;
	std::vector<TileInfo> MapTileInformation;

	GlobalConfigurations() {
		Scale = 10.0;
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

	





};

#endif
