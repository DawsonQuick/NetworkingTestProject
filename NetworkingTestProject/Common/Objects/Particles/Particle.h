#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
class Particle {
public:
	virtual bool isComplete() const = 0;
	virtual void onUpdate(float deltaTime) {};
	virtual float getPositionX() const = 0;
	virtual float getPositionY() const = 0;
	virtual ~Particle() {};
};
#endif