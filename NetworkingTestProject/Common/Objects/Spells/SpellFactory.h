#pragma once
#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H
#include "./Fireball.h"
#include "./MagicMissle.h"
#include "./CloudOfDaggers.h"
#include "./Spell.h"
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

	std::shared_ptr<Spell> stringNametoPointer(std::string name) {
		if (name == "MagicMissle") {
			return createMagicMissle();
		}
		else if (name == "Fireball") {
			return createFireball();
		}
		else if (name == "CloudOfDaggers") {
			return createCloudOfDaggers();
		}

		else {
			return nullptr;
		}
	}


};
#endif
