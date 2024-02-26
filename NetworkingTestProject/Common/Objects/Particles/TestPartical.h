#pragma once
#ifndef TESTPARTICAL_H
#define TESTPARTICAL_H
#include "Particle.h"
#include <random>
    std::random_device rd;
	std::mt19937 gen(rd());
class TestPartical : public Particle {
private:
	float posX;
	float posY;
	float velX;
	float velY;
	float lifetime;
	
public:

	//This constructor does not have a carry velocity , therefore it acts more as an explosions
	//Disriputes particales in all directs randomly
	TestPartical(float posX, float posY): posX(posX), posY(posY) {
	 std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
	 std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
	 velX = dist(gen);
	 velY = dist(gen);
	 lifetime = lifeTime(gen);
	}

	//This constructor passes in a velocity, the velocity is then weighted in the the randomness
	//Partical carry in the in the direction of the spawning partical
	TestPartical(float posX, float posY, float biasVelX, float biasVelY) : posX(posX), posY(posY) {
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(250.0f, 500.0f);
		velX = (((biasVelX)*(0.4)) + (dist(gen)*(0.6)))/2;
		velY = (((biasVelY)*(0.4)) + (dist(gen)*(0.6)))/2;
		lifetime = lifeTime(gen);
	}

	//Default Constructor
	TestPartical() {
		std::uniform_real_distribution<float> dist(-0.1f, 0.1f);
		std::uniform_real_distribution<float> lifeTime(3000.0f, 5000.0f);
		posX = 0.0f;
		posY = 0.0f;
		velX = dist(gen);
		velY = dist(gen);
		lifetime = lifeTime(gen);
	}

	~TestPartical() {

	}

	float getPositionX() const override {
		return posX;
	}
	float getPositionY() const override {
		return posY;
	}

	bool isComplete() const override {
		return (lifetime <= 0);
	}

	void onUpdate(float deltaTime) override{
		this->lifetime -= deltaTime;
		this->posX += (this->velX * deltaTime);
		this->posY += (this->velY * deltaTime);
	}


};
#endif