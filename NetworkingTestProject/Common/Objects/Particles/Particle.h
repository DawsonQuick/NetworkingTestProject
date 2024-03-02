#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include "./../../../OpenGL/vendor/glm/glm.hpp"
#include <functional>
#include <random>
#include "./../../Utils/TextureManager.h"
#include "./../../../Common/Utils/GlobalConfigurations.h"
std::random_device rd;
std::mt19937 gen(rd());
class Particle {
public:
	virtual bool isComplete() const = 0;
	virtual void onUpdate(float deltaTime) {};
	virtual float getPositionX() const = 0;
	virtual float getPositionY() const = 0;
	virtual glm::mat4x3 getVertexPos() const = 0;
	virtual glm::mat4x2 getTexturePos() const = 0;
	virtual ~Particle() {};
};
#endif