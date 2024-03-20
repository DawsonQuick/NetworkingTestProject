#pragma once
#ifndef QUARTERSTAFF_H
#define QUARTERSTAFF_H
#include "./../Weapon.h"
class Quarterstaff : public Weapon {
private:
	std::string name = "Quarterstaff";
	WeaponType type = WeaponType::SIMPLE_MELEE_WEAPON;
	std::vector<WeaponProperties> properties = { WeaponProperties::LIGHT };
	float range = 5.0f / 5.0f; // 1 square reach
	float impactRadius = 0.0f;
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

	~Quarterstaff() {};

};

#endif