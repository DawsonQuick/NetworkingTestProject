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

public:

	//Constructor
	GuidedParticle(float posX, float posY, float targetX, float targetY) :
		posX(posX), posY(posY), targetX(targetX),  targetY(targetY) {
		float directionX = this->targetX - this->posX;
		float directionY = this->targetY- this->posY;
		float angle = atan2(directionY, directionX);
		velX = cos(angle) / 5;
		velY = sin(angle) / 5;
	}

	//Default Constructor
	GuidedParticle() {
		float directionX = 0.0 - 0.0;
		float directionY = 0.0 - 0.0;
		float angle = atan2(directionY, directionX);
		velX = cos(angle);
		velY = sin(angle);
	}

	~GuidedParticle() {

	}

	float getPositionX() const override {
		return posX;
	}
	float getPositionY() const override {
		return posY;
	}

	bool isComplete() const override {
		bool complete = false;
		float distance = std::sqrt(std::pow(targetX - posX, 2) + std::pow(targetY - posY, 2));
		if (distance <= 1.0) {
			//Example of chaining particles
			for (int i = 0; i < 100; i++) {
				std::unique_ptr<Particle> particle = std::make_unique<TestPartical>(posX, posY,velX,velY);
				ParticalDatabase::getInstance().addParticle("Test" + std::to_string(i), std::move(particle));
			}
		}
		return (distance <= 1.0);
	}

	void onUpdate(float deltaTime) override {
		this->lifetime -= deltaTime;
		this->posX += (this->velX * deltaTime);
		this->posY += (this->velY * deltaTime);
	}


};
#endif
