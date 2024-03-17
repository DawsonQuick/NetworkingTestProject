#pragma once
#ifndef PARTICLEFACTORY_H
#define PARTICLEFACTORY_H
#include "./GuidedParticle.h"
#include "./TestParticle.h"
#include "./ImplosionParticle.h"
#include "./CloudOfDaggersParticle.h"

#include "./../../Utils/ParticalDataBase.h"
#include <memory>
class ParticleFactory {

private:
	// Delete copy constructor and assignment operator
	ParticleFactory(const ParticleFactory&) = delete;
	ParticleFactory& operator=(const ParticleFactory&) = delete;

	ParticleFactory() {
	};

public:
	// Singleton Constructor
	static ParticleFactory& getInstance() {
		static ParticleFactory instance;
		return instance;
	}
	std::shared_ptr<GuidedParticle> createGuidedParticle(float posX, float posY, float targetX, float targetY, float scale, std::function<void(float, float)> Tmpcallback) {
		return std::make_shared<GuidedParticle>(posX, posY,targetX,targetY, scale, Tmpcallback);
	}
	std::shared_ptr<ImplosionParticle> createImplosionParticle(float posX, float posY, float scale) {
		return std::make_shared<ImplosionParticle>(posX, posY, scale);
	}
	std::shared_ptr<TestParticle> createTestParticle(float posX, float posY, float biasVelX, float biasVelY, float scale) {
		return std::make_shared<TestParticle>(posX, posY, biasVelX, biasVelY, scale);
	}
	std::shared_ptr<CloudOfDaggersParticle> createCloudOfDaggersParticle(float posX, float posY, float range, float tmpLifeTime, float scale) {
		return std::make_shared<CloudOfDaggersParticle>(posX, posY, range, tmpLifeTime, scale);
	}

};
#endif
