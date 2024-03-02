#pragma once
#ifndef GUIDEDPARTICLE_H
#define GUIDEDPARTICLE_H
#include "Particle.h"
#include "./../../Utils/ParticalDataBase.h"
#include <random>
#include <cmath>
#include <iostream>
//Point to point particle. (Projectile style)
class GuidedParticle : public Particle {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float lifetime;

	float targetX;
	float targetY;
	glm::mat4x3 vertexPos = {
		glm::vec3(-0.5f,-0.5,0.0),
		glm::vec3(0.5,-0.5,0.0),
		glm::vec3(0.5,0.5,0.0),
		glm::vec3(-0.5,0.5,0.0),
	};
	glm::mat4x2 texturePos = TextureManager::getInstance().getTextureInfo(typeid(GuidedParticle));
	std::function<void(float,float)> callback;

public:

	//Constructor
	GuidedParticle(float posX, float posY, float targetX, float targetY , float scale , std::function<void(float, float)> Tmpcallback) :
		posX(posX), posY(posY), targetX(targetX),  targetY(targetY) {
		float directionX = this->targetX - this->posX;
		float directionY = this->targetY- this->posY;
		float angle = atan2(directionY, directionX);
		velX = cos(angle) / (GlobalConfigurations::getInstance().getScale()*2);
		velY = sin(angle) / (GlobalConfigurations::getInstance().getScale()*2);
		vertexPos = vertexPos * scale * (GlobalConfigurations::getInstance().getScale());
		callback = Tmpcallback;
		updateSpriteRotation();
	}

	//Default Constructor
	GuidedParticle() {
		float directionX = 0.0 - 0.0;
		float directionY = 0.0 - 0.0;
		float angle = atan2(directionY, directionX);
		velX = cos(angle);
		velY = sin(angle);
		callback = nullptr;
		updateSpriteRotation();
	}

	~GuidedParticle() {

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
		bool complete = false;
		float distance = std::sqrt(std::pow(targetX - posX, 2) + std::pow(targetY - posY, 2));
		if (distance <= 1.0 *(GlobalConfigurations::getInstance().getScale()/2)) {
			if (callback) {
				callback(posX,posY);
			}
			//Example of chaining particles
			for (int i = 0; i < 100; i++) {
				std::unique_ptr<Particle> particle = std::make_unique<ImplosionParticle>(posX, posY,0.3f);
				ParticalDatabase::getInstance().addParticle("Test" + std::to_string(i), std::move(particle));
			}
		}
		return (distance <= 1.0 * (GlobalConfigurations::getInstance().getScale() / 2));
	}

	void onUpdate(float deltaTime) override {
		this->lifetime -= deltaTime;
		this->posX += (this->velX * deltaTime * (GlobalConfigurations::getInstance().getScale()/2));
		this->posY += (this->velY * deltaTime * (GlobalConfigurations::getInstance().getScale()/2));
		
	}


	void updateSpriteRotation() {
		// Calculate angle of rotation based on velocity
		float angle = atan2(this->velY, this->velX);

		// Additional rotation angle (270 degrees or -90 degrees)
		float additionalAngle = glm::radians(270.0f); // 270 degrees in radians

		// Apply rotation to vertexPos
		for (int i = 0; i < 4; ++i) {
			// Get current vertex position
			float x = vertexPos[i].x;
			float y = vertexPos[i].y;

			// Rotate the vertex around the z-axis
			// Additional rotation applied here
			float rotatedX = x * cos(angle + additionalAngle) - y * sin(angle + additionalAngle);
			float rotatedY = x * sin(angle + additionalAngle) + y * cos(angle + additionalAngle);

			vertexPos[i].x = rotatedX;
			vertexPos[i].y = rotatedY;
		}
	}


};
#endif
