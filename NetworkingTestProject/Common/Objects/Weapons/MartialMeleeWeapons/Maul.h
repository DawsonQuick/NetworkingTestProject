#pragma once
#ifndef MAUL_H
#define MAUL_H
#include "./../Weapon.h"
class Maul : public Weapon {
private:
	std::string name = "Maul";
	WeaponType type = WeaponType::MARTIAL_MELEE_WEAPON;
	std::vector<WeaponProperties> properties = { WeaponProperties::HEAVY,WeaponProperties::TWO_HANDED };
	float range = 5.0f / 5.0f; // 1 square reach
	float impactRadius = 0.0f;

	std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
	{2,DiceType::D6,DamageTypes::BLUDGEONING},
	};

public:
	std::string getName() const override {
		return name;
	}
	float getImpactRadius() const override {
		return impactRadius;
	}
	float getRange() const override {
		return range;
	}
	WeaponType getWeaponType() const override {
		return type;
	}
	std::vector<WeaponProperties> getWeaponProperties() const override {
		return properties;
	}
	void attack(std::string playerName, float targetPosX, float targetPosY) override {
		//Do Stuff Here
	}

	~Maul() {};

};

#endif