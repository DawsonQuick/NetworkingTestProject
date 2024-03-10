#pragma once
#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H
#include "./Fireball.h"
#include "./MagicMissle.h"
#include "./CloudOfDaggers.h"
#include <memory>
class SpellFactory {

private:
	// Delete copy constructor and assignment operator
	SpellFactory(const SpellFactory&) = delete;
	SpellFactory& operator=(const SpellFactory&) = delete;

	SpellFactory() {
	};

public:
	// Singleton Constructor
	static SpellFactory& getInstance() {
		static SpellFactory instance;
		return instance;
	}

	std::shared_ptr<MagicMissle> createMagicMissle() {
		return std::make_shared<MagicMissle>();
	}
	std::shared_ptr<Fireball> createFireball() {
		return std::make_shared<Fireball>();
	}
	std::shared_ptr<CloudOfDaggers> createCloudOfDaggers() {
		return std::make_shared<CloudOfDaggers>();
	}




};
#endif
