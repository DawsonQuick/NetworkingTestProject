#pragma once
#ifndef LIGHTCROSSBOW_H
#define LIGHTCROSSBOW_H
#include "./../Weapon.h"
class Lightcrossbow : public Weapon {
private:
  std::string name = "Lightcrossbow";
  WeaponType type = WeaponType::SIMPLE_RANGED_WEAPON;
  std::vector<WeaponProperties> properties = {WeaponProperties::TWO_HANDED};
  float range = 5.0f / 5.0f; // 1 square reach
  float impactRadius = 0.0f;


  std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
    {1,DiceType::D8,DamageTypes::PIERCING},
  };


public:
  std::string getName() const override { return name; }
  float getImpactRadius() const override { return impactRadius; }
  float getRange() const override { return range; }
  WeaponType getWeaponType() const override { return type; }
  std::vector<WeaponProperties> getWeaponProperties() const override {
    return properties;
  }
  void attack(std::string playerName, float targetPosX,
              float targetPosY) override {
    // Do Stuff Here
  }

  ~Lightcrossbow(){};
};

#endif