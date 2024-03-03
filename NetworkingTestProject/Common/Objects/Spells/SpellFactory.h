#pragma once
#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H
#include "./Spell.h"
#include "./Fireball.h"
#include "./MagicMissle.h"
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



};
#endif
