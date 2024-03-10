#pragma once
#ifndef CLOUDOFDAGGERSPARTICLE_H
#define CLOUDOFDAGGERSPARTICLE_H
#include "Particle.h"
#include "./../../Utils/ParticalDataBase.h"
#include <random>
#include <cmath>
#include <iostream>
//Point to point particle. (Projectile style)
class CloudOfDaggersParticle : public Particle {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float originX;
	float originY;
	float lifetime;
	float currentAngle;
	float radius;
	float rotationSpeed;
	glm::mat4x3 vertexPos = {
		glm::vec3(-0.5f,-0.5,0.0),
		glm::vec3(0.5,-0.5,0.0),
		glm::vec3(0.5,0.5,0.0),
		glm::vec3(-0.5,0.5,0.0),
	};

	glm::mat4x3 basePos = {
	glm::vec3(-0.5f,-0.5,0.0),
	glm::vec3(0.5,-0.5,0.0),
	glm::vec3(0.5,0.5,0.0),
	glm::vec3(-0.5,0.5,0.0),
	};
	glm::mat4x2 texturePos = TextureManager::getInstance().getTextureInfo(typeid(CloudOfDaggersParticle));
	std::function<void(float, float)> callback;

public:

	//Constructor
	CloudOfDaggersParticle(float targetX, float targetY,float range,float tmpLifeTime, float scale) :
		posX(targetX), posY(targetY), originX(targetX), originY(targetY){
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		//std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
		std::uniform_real_distribution<float> startingAngle(0.0f, 360.0f);
		std::uniform_real_distribution<float> randomRadius(range/2, range*2);
		std::uniform_real_distribution<float> randomRotationSpeed(1.0f, 3.0f);
		//lifetime = lifeTime(gen);
		lifetime = tmpLifeTime;
		vertexPos = vertexPos * scale * GlobalConfigurations::getInstance().getScale();
		basePos = basePos * scale * GlobalConfigurations::getInstance().getScale();
		currentAngle = startingAngle(gen);
		updateSpriteRotation(currentAngle);
		radius = randomRadius(gen);
		rotationSpeed = randomRotationSpeed(gen);
		//updateSpriteRotation();
	}

	//Default Constructor
	CloudOfDaggersParticle() {
		float directionX = 0.0 - 0.0;
		float directionY = 0.0 - 0.0;
		float angle = atan2(directionY, directionX);
		//velX = cos(angle);
		//velY = sin(angle);
		callback = nullptr;
		//updateSpriteRotation();
	}

	~CloudOfDaggersParticle() {

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
		currentAngle = currentAngle + rotationSpeed;
		// Convert degrees to radians
		double angleRadians = currentAngle * 3.1415 / 180.0;
		this->posX = originX + cos(angleRadians) * (radius * GlobalConfigurations::getInstance().getScale());
		this->posY = originY + sin(angleRadians) * (radius * GlobalConfigurations::getInstance().getScale());
		

		float angleToCenter = std::atan2(posY - originY, posX - originX);
		float tangentAngle = angleToCenter + 3.1415 / 2.0f;
		updateSpriteRotation(tangentAngle);


	}

	
	void updateSpriteRotation(float angle) {

		// Additional rotation angle (270 degrees or -90 degrees)
		float additionalAngle = glm::radians(270.0f); // 270 degrees in radians

		// Apply rotation to vertexPos
		for (int i = 0; i < 4; ++i) {
			// Get current vertex position
			float x = basePos[i].x;
			float y = basePos[i].y;

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
