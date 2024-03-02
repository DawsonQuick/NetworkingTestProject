#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <typeindex>
#include "./../../OpenGL/Utils/Texture.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"

class TextureManager {
private:
	std::map<std::type_index, glm::mat4x2>classTextureDatabase;

	TextureManager() {}

	// Delete copy constructor and assignment operator
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

public:
	// Singleton Constructor
	static TextureManager& getInstance() {
		static TextureManager instance;
		return instance;
	}

	//Load the ClassID to texture positions in from the "TextureMapping.h"
	void setTextureDatabase(std::map<std::type_index, glm::mat4x2> textureMap) {
		classTextureDatabase = textureMap;
	}

	//Classes will call this file to get their textures
	glm::mat4x2 getTextureInfo(const std::type_info& classType) {
		auto it = classTextureDatabase.find(classType);
		if (it != classTextureDatabase.end()) {
			return it->second;
		}
		else {
			return glm::mat4x2{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
		}
	}



};

#endif