#pragma once
#ifndef IMPLOSIONPARTICAL_H
#define IMPLOSIONPARTICAL_H
#include "Particle.h"
#include <random>
class ImplosionParticle : public Particle {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float lifetime;
	float halfLife;
	glm::mat4x2 texturePos = TextureManager::getInstance().getTextureInfo(typeid(ImplosionParticle));

	glm::mat4x3 vertexPos = {
			glm::vec3(-0.5f,-0.5,0.0),
			glm::vec3(0.5,-0.5,0.0),
			glm::vec3(0.5,0.5,0.0),
			glm::vec3(-0.5,0.5,0.0),
	};
public:

	//This constructor does not have a carry velocity , therefore it acts more as an explosions
	//Disriputes particales in all directs randomly
	ImplosionParticle(float posX, float posY, float scale) : posX(posX), posY(posY) {

		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
		velX = dist(gen)/2;
		velY = dist(gen)/2;
		lifetime = lifeTime(gen);
		halfLife = lifetime / 2;
		vertexPos = vertexPos * scale * (GlobalConfigurations::getInstance().getScale()/2);
	}

	//Default Constructor
	ImplosionParticle() {
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(3000.0f, 5000.0f);
		posX = 0.0f;
		posY = 0.0f;
		velX = dist(gen);
		velY = dist(gen);
		lifetime = lifeTime(gen);
	}

	~ImplosionParticle() {

	}

	float getPositionX() const override {
		return posX;
	}
	float getPositionY() const override {
		return posY;
	}
	glm::mat4x3 getVertexPos() const override {
		return vertexPos;
	}

	glm::mat4x2 getTexturePos() const override {
		return texturePos;
	}

	bool isComplete() const override {
		return (lifetime <= 0);
	}


	void onUpdate(float deltaTime) override {
		this->lifetime -= deltaTime;
		if (lifetime < halfLife) {
			this->posX -= (this->velX * deltaTime * GlobalConfigurations::getInstance().getScale()/2);
			this->posY -= (this->velY * deltaTime * GlobalConfigurations::getInstance().getScale()/2);
		}
		else {
			this->posX += (this->velX * deltaTime * GlobalConfigurations::getInstance().getScale()/2);
			this->posY += (this->velY * deltaTime * GlobalConfigurations::getInstance().getScale()/2);
		}
		updateSpriteRotation();

	}

	void updateSpriteRotation() {
		// Calculate angle of rotation based on velocity
		float angle = atan2(this->velY, this->velX);

		// Apply rotation to vertexPos
		for (int i = 0; i < 4; ++i) {
			// Get current vertex position
			float x = vertexPos[i].x;
			float y = vertexPos[i].y;

			// Rotate the vertex around the z-axis
			vertexPos[i].x = x * cos(angle) - y * sin(angle);
			vertexPos[i].y = x * sin(angle) + y * cos(angle);
		}
	}

};
#endif
