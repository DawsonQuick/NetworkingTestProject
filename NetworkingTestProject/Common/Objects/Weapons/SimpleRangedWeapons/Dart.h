#pragma once
#ifndef DART_H
#define DART_H
#include "./../Weapon.h"
class Dart : public Weapon {
private:
  std::string name = "Dart";
  WeaponType type = WeaponType::SIMPLE_RANGED_WEAPON;
  std::vector<WeaponProperties> properties = {WeaponProperties::FINESSE, WeaponProperties::THROWN};
  float range = 5.0f / 5.0f; // 1 square reach
  float impactRadius = 0.0f;

  std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
    {1,DiceType::D4,DamageTypes::PIERCING},
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

  ~Dart(){};
};

#endif