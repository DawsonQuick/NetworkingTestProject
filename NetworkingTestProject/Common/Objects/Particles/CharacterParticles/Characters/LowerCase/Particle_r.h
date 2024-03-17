#pragma once
#ifndef PARTICLE_r_H
#define PARTICLE_r_H
#include "./../../../Particle.h"
class Particle_r : public Particle {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float lifetime;
	glm::mat4x2 texturePos = TextureManager::getInstance().getTextureInfo(typeid(Particle_r));

	glm::mat4x3 vertexPos = {
			glm::vec3(-0.5f,-0.5,0.0),
			glm::vec3(0.5,-0.5,0.0),
			glm::vec3(0.5,0.5,0.0),
			glm::vec3(-0.5,0.5,0.0),
	};
public:

	//This constructor does not have a carry velocity , therefore it acts more as an explosions
	//Disriputes particales in all directs randomly
	Particle_r(float posX, float posY, float scale) : posX(posX), posY(posY) {
		velX = 0.0;
		velY = 0.001;
		lifetime = 2000; //2 Seconds
		vertexPos = vertexPos * scale * (GlobalConfigurations::getInstance().getScale() / 2);
	}

	//Default Constructor
	Particle_r() {
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(3000.0f, 5000.0f);
		posX = 0.0f;
		posY = 0.0f;
		velX = dist(gen);
		velY = dist(gen);
		lifetime = lifeTime(gen);
	}

	~Particle_r() {

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
		this->posX += (this->velX * deltaTime * GlobalConfigurations::getInstance().getScale() / 2);
		this->posY += (this->velY * deltaTime * GlobalConfigurations::getInstance().getScale() / 2);

	}

};
#endif
