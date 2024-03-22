#pragma once
#ifndef WEAPONFACTORY_H
#define WEAPONFACTORY_H
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
#include "Weapon.h"
#include <memory>

class WeaponFactory {

private:
  // Delete copy constructor and assignment operator
  WeaponFactory(const WeaponFactory &) = delete;
  WeaponFactory &operator=(const WeaponFactory &) = delete;

  WeaponFactory(){};

public:
  // Singleton Constructor
  static WeaponFactory &getInstance() {
    static WeaponFactory instance;
    return instance;
  }

  // Simple Melee Weapons
  std::shared_ptr<Club> createClub() { return std::make_shared<Club>(); }
  std::shared_ptr<Dagger> createDagger() { return std::make_shared<Dagger>(); }
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
  std::shared_ptr<Mace> createMace() { return std::make_shared<Mace>(); }
  std::shared_ptr<Quarterstaff> createQuarterstaff() {
    return std::make_shared<Quarterstaff>();
  }
  std::shared_ptr<Sickle> createSickle() { return std::make_shared<Sickle>(); }
  std::shared_ptr<Spear> createSpear() { return std::make_shared<Spear>(); }

  // Simple Ranged Weapons
  std::shared_ptr<Dart> createDart() { return std::make_shared<Dart>(); }
  std::shared_ptr<Lightcrossbow> createLightcrossbow() {
    return std::make_shared<Lightcrossbow>();
  }
  std::shared_ptr<Shortbow> createShortbow() {
    return std::make_shared<Shortbow>();
  }
  std::shared_ptr<Sling> createSling() { return std::make_shared<Sling>(); }

  // Martial Melee Weapons

  std::shared_ptr<Battleaxe> createBattleaxe() {
    return std::make_shared<Battleaxe>();
  }
  std::shared_ptr<Flail> createFlail() { return std::make_shared<Flail>(); }
  std::shared_ptr<Glaive> createGlaive() { return std::make_shared<Glaive>(); }
  std::shared_ptr<Greatsword> createGreatsword() {
    return std::make_shared<Greatsword>();
  }
  std::shared_ptr<Halberd> createHalberd() {
    return std::make_shared<Halberd>();
  }
  std::shared_ptr<Lance> createLance() { return std::make_shared<Lance>(); }
  std::shared_ptr<Longsword> createLongsword() {
    return std::make_shared<Longsword>();
  }
  std::shared_ptr<Maul> createMaul() { return std::make_shared<Maul>(); }
  std::shared_ptr<Morningstar> createMorningstar() {
    return std::make_shared<Morningstar>();
  }
  std::shared_ptr<Pike> createPike() { return std::make_shared<Pike>(); }
  std::shared_ptr<Rapier> createRapier() { return std::make_shared<Rapier>(); }
  std::shared_ptr<Scimitar> createScimitar() {
    return std::make_shared<Scimitar>();
  }
  std::shared_ptr<Shortsword> createShortsword() {
    return std::make_shared<Shortsword>();
  }
  std::shared_ptr<Trident> createTrident() {
    return std::make_shared<Trident>();
  }
  std::shared_ptr<Warhammer> createWarhammer() {
    return std::make_shared<Warhammer>();
  }
  std::shared_ptr<Warpick> createWarpick() {
    return std::make_shared<Warpick>();
  }
  std::shared_ptr<Whip> createWhip() { return std::make_shared<Whip>(); }

  // Marial Ranged
  std::shared_ptr<Blowgun> createBlowgun() {
    return std::make_shared<Blowgun>();
  }
  std::shared_ptr<Handcrossbow> createHandcrossbow() {
    return std::make_shared<Handcrossbow>();
  }
  std::shared_ptr<Heavycrossbow> createHeavycrossbow() {
    return std::make_shared<Heavycrossbow>();
  }
  std::shared_ptr<Longbow> createLongbow() {
    return std::make_shared<Longbow>();
  }

  std::shared_ptr<Weapon> stringNametoPointer(std::string name) {
    // Simple Melee Weapons
    if (name == "Club") {
      return createClub();
    } else if (name == "Dagger") {
      return createDagger();
    } else if (name == "Greatclub") {
      return createGreatclub();
    } else if (name == "Handaxe") {
      return createHandaxe();
    } else if (name == "Javelin") {
      return createJavelin();
    } else if (name == "Lighthammer") {
      return createLighthammer();
    } else if (name == "Mace") {
      return createMace();
    } else if (name == "Quarterstaff") {
      return createQuarterstaff();
    } else if (name == "Sickle") {
      return createSickle();
    } else if (name == "Spear") {
      return createSpear();
    }

    // Simple Ranged Weapons
    else if (name == "Dart") {
      return createDart();
    } else if (name == "Lightcrossbow") {
      return createLightcrossbow();
    } else if (name == "Shortbow") {
      return createShortbow();
    } else if (name == "Sling") {
      return createSling();
    }

    // Martial Melee Weapons
    else if (name == "Battleaxe") {
      return createBattleaxe();
    } else if (name == "Flail") {
      return createFlail();
    } else if (name == "Glaive") {
      return createGlaive();
    } else if (name == "Greatsword") {
      return createGreatsword();
    } else if (name == "Halberd") {
      return createHalberd();
    } else if (name == "Lance") {
      return createLance();
    } else if (name == "Longsword") {
      return createLongsword();
    } else if (name == "Maul") {
      return createMaul();
    } else if (name == "Morningstar") {
      return createMorningstar();
    } else if (name == "Pike") {
      return createPike();
    } else if (name == "Rapier") {
      return createRapier();
    } else if (name == "Scimitar") {
      return createScimitar();
    } else if (name == "Shortsword") {
      return createShortsword();
    } else if (name == "Trident") {
      return createTrident();
    } else if (name == "Warhammer") {
      return createWarhammer();
    } else if (name == "Warpick") {
      return createWarpick();
    } else if (name == "Whip") {
      return createWhip();
    }

    // Martial Ranged Weapons
    else if (name == "Blowgun") {
      return createBlowgun();
    } else if (name == "Handcrossbow") {
      return createHandcrossbow();
    } else if (name == "Heavycrossbow") {
      return createHeavycrossbow();
    } else if (name == "Longbow") {
      return createLongbow();
    } else if (name == "Sling") {
      return createSling();
    }

    else {
      return nullptr;
    }
  }
};
#endif