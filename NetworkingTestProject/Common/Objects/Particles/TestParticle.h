#pragma once
#ifndef TESTPARTICAL_H
#define TESTPARTICAL_H
#include "Particle.h"

class TestParticle : public Particle {
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
	glm::mat4x2 texturePos = TextureManager::getInstance().getTextureInfo(typeid(TestParticle));
	glm::mat4x3 vertexPos = {
			glm::vec3(-0.5f,-0.5,0.0),
			glm::vec3(0.5,-0.5,0.0),
			glm::vec3(0.5,0.5,0.0),
			glm::vec3(-0.5,0.5,0.0),
		};
public:

	//This constructor does not have a carry velocity , therefore it acts more as an explosions
	//Disriputes particales in all directs randomly
	TestParticle(float posX, float posY, float scale): posX(posX), posY(posY), originX(posX), originY(posY){

	 std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
	 std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
	 velX = dist(gen);
	 velY = dist(gen);
	 lifetime = lifeTime(gen);
	 vertexPos = vertexPos * scale;
	 currentAngle = 0.0f;

	}

	//This constructor passes in a velocity, the velocity is then weighted in the the randomness
	//Partical carry in the in the direction of the spawning partical
	TestParticle(float posX, float posY, float biasVelX, float biasVelY , float scale) : posX(posX), posY(posY), originX(posX), originY(posY) {

		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
		std::uniform_real_distribution<float> startingAngle(0.0f, 360.0f);
		std::uniform_real_distribution<float> randomRadius(0.0f, 5.0f);
		std::uniform_real_distribution<float> randomRotationSpeed(0.0f, 5.0f);
		velX = (((biasVelX)*(0.3)) + (dist(gen)*(0.7)))/2;
		velY = (((biasVelY)*(0.3)) + (dist(gen)*(0.7)))/2;
		lifetime = lifeTime(gen);
		vertexPos = vertexPos * scale * GlobalConfigurations::getInstance().getScale();
		currentAngle = startingAngle(gen);
		radius = randomRadius(gen);
		rotationSpeed = randomRotationSpeed(gen);
	}

	//Default Constructor
	TestParticle() {
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(3000.0f, 5000.0f);
		posX = 0.0f;
		posY = 0.0f;
		velX = dist(gen);
		velY = dist(gen);
		lifetime = lifeTime(gen);
	}

	~TestParticle() {

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


	void onUpdate(float deltaTime) override{
		this->lifetime -= deltaTime;
		currentAngle = currentAngle + rotationSpeed;
		// Convert degrees to radians
		double angleRadians = currentAngle * 3.1415 / 180.0;
		this->posX = originX + cos(angleRadians) * (radius * GlobalConfigurations::getInstance().getScale());
		this->posY = originY + sin(angleRadians) * (radius * GlobalConfigurations::getInstance().getScale());
	}


};
#endif