#pragma once
#include <typeindex>
#include <map>
#include "./TextureManager.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"
#include "./../../Common/Objects/Particles/GuidedParticle.h"
#include "./../../Common/Objects/Particles/ImplosionParticle.h"
#include "./../../Common/Objects/Particles/TestParticle.h"
#include "./../../Common/Objects/Particles/CloudOfDaggersParticle.h"


glm::mat4x2 getTexturePositions(glm::vec2 position);


void setPositions() {
std::map<std::type_index, glm::mat4x2> texturePositions;
	//Particles
	texturePositions.emplace(typeid(GuidedParticle),				getTexturePositions(glm::vec2(0.0,0.0)));
	texturePositions.emplace(typeid(ImplosionParticle),				getTexturePositions(glm::vec2(0.0,3.0)));
	texturePositions.emplace(typeid(TestParticle),					getTexturePositions(glm::vec2(0.0,3.0)));
	texturePositions.emplace(typeid(CloudOfDaggersParticle),		getTexturePositions(glm::vec2(0.0,4.0)));
	TextureManager::getInstance().setTextureDatabase(texturePositions);
}



glm::mat4x2 getTexturePositions(glm::vec2 position) {
	// Width and height of the entire texture
	float textureWidth = 1920.0f;
	float textureHeight = 1080.0f;

	float onePixelWidth = 1 / textureWidth;
	float onePixelHeight = 1 / textureHeight;

	// Width and height of the sub-texture
	float subTextureWidth = 64.0f;
	float subTextureHeight = 64.0f;


	// Calculate texture coordinates for the sub-texture
	float texCoordLeft = (subTextureWidth * position.y) / textureWidth;               // Left edge of the sub-texture
	float texCoordRight = ((position.y + 1) * subTextureWidth) / textureWidth;         // Right edge of the sub-texture
	float texCoordTop = 1.0f - ((subTextureHeight * position.x) / textureHeight);        // Top edge of the sub-texture
	float texCoordBottom = 1.0f - (((position.x + 1) * (subTextureHeight)) / textureHeight); // Bottom edge of the sub-texture

	float Text00 = texCoordLeft + onePixelWidth;		//TextureX
	float Text01 = texCoordBottom + onePixelHeight;		//TextureY

	float Text10 = texCoordRight - onePixelWidth;		//TextureX
	float Text11 = texCoordBottom + onePixelHeight;		//TextureY


	float Text20 = texCoordRight - onePixelWidth;		//TextureX
	float Text21 = texCoordTop - onePixelHeight;		//TextureY

	float Text30 = texCoordLeft + onePixelWidth;		//TextureX
	float Text31 = texCoordTop - onePixelHeight;		//TextureY


	return glm::mat4x2{ Text00 ,Text01,Text10,Text11,Text20,Text21,Text30,Text31 };
}

