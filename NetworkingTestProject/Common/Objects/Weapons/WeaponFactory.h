#pragma once
#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H
#include <memory>
#include "Weapon.h"
#include "./SimpleMeleeWeapons/Club.h"
#include "./SimpleMeleeWeapons/Dagger.h"
#include "./SimpleMeleeWeapons/Greatclub.h"
#include "./SimpleMeleeWeapons/Handaxe.h"
#include "./SimpleMeleeWeapons/Javelin.h"
#include "./SimpleMeleeWeapons/Lighthammer.h"
#include "./SimpleMeleeWeapons/Mace.h"
#include "./SimpleMeleeWeapons/Quarterstaff.h"
#include "./SimpleMeleeWeapons/Sickle.h"
#include "./SimpleMeleeWeapons/Spear.h"
#include "./SimpleRangedWeapons/Dart.h"
#include "./SimpleRangedWeapons/LightCrossbow.h"
#include "./SimpleRangedWeapons/Shortbow.h"
#include "./SimpleRangedWeapons/Sling.h"
#include "./MartialMeleeWeapons/Battleaxe.h"
#include "./MartialMeleeWeapons/Flail.h"
#include "./MartialMeleeWeapons/Glaive.h"
#include "./MartialMeleeWeapons/Greatsword.h"
#include "./MartialMeleeWeapons/Halberd.h"
#include "./MartialMeleeWeapons/Lance.h"
#include "./MartialMeleeWeapons/Longsword.h"
#include "./MartialMeleeWeapons/Maul.h"
#include "./MartialMeleeWeapons/Morningstar.h"
#include "./MartialMeleeWeapons/Pike.h"
#include "./MartialMeleeWeapons/Rapier.h"
#include "./MartialMeleeWeapons/Scimitar.h"
#include "./MartialMeleeWeapons/Shortsword.h"
#include "./MartialMeleeWeapons/Trident.h"
#include "./MartialMeleeWeapons/Warhammer.h"
#include "./MartialMeleeWeapons/Warpick.h"
#include "./MartialMeleeWeapons/Whip.h"
#include "./MartialRangedWeapons/Blowgun.h"
#include "./MartialRangedWeapons/Handcrossbow.h"
#include "./MartialRangedWeapons/Heavycrossbow.h"
#include "./MartialRangedWeapons/Longbow.h"
#include "./MartialRangedWeapons/Sling.h"

class WeaponFactory {

private:
	// Delete copy constructor and assignment operator
	WeaponFactory(const WeaponFactory&) = delete;
	WeaponFactory& operator=(const WeaponFactory&) = delete;

	WeaponFactory() {
	};

public:
	// Singleton Constructor
	static WeaponFactory& getInstance() {
		static WeaponFactory instance;
		return instance;
	}

	std::shared_ptr<Club> createClub() {
		return std::make_shared<Club>();
	}
	std::shared_ptr<Dagger> createDagger() {
		return std::make_shared<Dagger>();
	}
	std::shared_ptr<Greatclub> createGreatclub() {
		return std::make_shared<Greatclub>();
	}
	std::shared_ptr<Handaxe> createHandaxe() {
		return std::make_shared<Handaxe>();
	}
	std::shared_ptr<Javelin> createJavelin() {
		return std::make_shared<Javelin>();
	}
	std::shared_ptr<Lighthammer> createLighthammer() {
		return std::make_shared<Lighthammer>();
	}
	std::shared_ptr<Mace> createMace() {
		return std::make_shared<Mace>();
	}
	std::shared_ptr<Quarterstaff> createQuarterstaff() {
		return std::make_shared<Quarterstaff>();
	}
	std::shared_ptr<Sickle> createSickle() {
		return std::make_shared<Sickle>();
	}
	std::shared_ptr<Spear> createSpear() {
		return std::make_shared<Spear>();
	}
	

	std::shared_ptr<Weapon> stringNametoPointer(std::string name) {
		if (name == "Club") {
			return createClub ();
		}
		else if (name == "Dagger") {
			return createDagger();
		}
		else if (name == "Greatclub") {
			return createGreatclub();
		}
		else if (name == "Handaxe") {
			return createHandaxe();
		}
		else if (name == "Javelin") {
			return createJavelin();
		}
		else if (name == "Lighthammer") {
			return createLighthammer();
		}
		else if (name == "Mace") {
			return createMace();
		}
		else if (name == "Quarterstaff") {
			return createQuarterstaff();
		}
		else if (name == "Sickle") {
			return createSickle();
		}
		else if (name == "Spear") {
			return createSpear();
		}

		else {
			return nullptr;
		}
	}


};
#endif