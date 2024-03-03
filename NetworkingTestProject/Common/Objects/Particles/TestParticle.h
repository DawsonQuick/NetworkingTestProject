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
	float lifetime;
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
	TestParticle(float posX, float posY, float scale): posX(posX), posY(posY) {

	 std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
	 std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
	 velX = dist(gen);
	 velY = dist(gen);
	 lifetime = lifeTime(gen);
	 vertexPos = vertexPos * scale;
	}

	//This constructor passes in a velocity, the velocity is then weighted in the the randomness
	//Partical carry in the in the direction of the spawning partical
	TestParticle(float posX, float posY, float biasVelX, float biasVelY , float scale) : posX(posX), posY(posY) {

		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
		velX = (((biasVelX)*(0.3)) + (dist(gen)*(0.7)))/2;
		velY = (((biasVelY)*(0.3)) + (dist(gen)*(0.7)))/2;
		lifetime = lifeTime(gen);
		vertexPos = vertexPos * scale * GlobalConfigurations::getInstance().getScale();
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
		this->posX += (this->velX * deltaTime * GlobalConfigurations::getInstance().getScale());
		this->posY += (this->velY * deltaTime * GlobalConfigurations::getInstance().getScale());
	}


};
#endif