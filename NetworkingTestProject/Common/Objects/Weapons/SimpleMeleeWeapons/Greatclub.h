#pragma once
#ifndef GREATCLUB_H
#define GREATCLUB_H
#include "./../Weapon.h"
class Greatclub : public Weapon {
private:
  std::string name = "Greatclub";
  WeaponType type = WeaponType::SIMPLE_MELEE_WEAPON;
  std::vector<WeaponProperties> properties = {WeaponProperties::TWO_HANDED};
  float range = 5.0f / 5.0f; // 1 square reach
  float impactRadius = 0.0f;

  std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
      {1,DiceType::D8,DamageTypes::BLUDGEONING},
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
      if (playerName == GlobalConfigurations::getInstance().getPlayerName()) {
          //Sample hit detection
          for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
              float playerX = player.second.getPositionX();
              float playerY = player.second.getPositionY();

              if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                  //Circle:   PosX        PosY           Circle Radius # of Grid cells * Scale of cells      Player : Bottom-Left vertex position , Top-Right vertex position
                  if (RectangleVsCircle(targetPosX, targetPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), (playerX - 0.5f), (playerY - 0.5f), (playerX + 0.5f), (playerY + 0.5f))) {
                      int currentPlayerHealth = player.second.getHealth();
                      std::vector<int> attackersModifiers = { 2 };
                      if (doesAttackLand(attackersModifiers, player.second.getAC())) {
                          std::vector<std::tuple<int, DamageTypes>> incomingDamage;
                          incomingDamage = calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());
                          for (std::tuple<int, DamageTypes> damage : incomingDamage) {
                              int tmpDamage = std::get<0>(damage);
                              CharacterParticleFactory::getInstance().generateTextParticle(playerX, playerY, "-" + std::to_string(tmpDamage), std::get<1>(damage));
                              currentPlayerHealth -= tmpDamage;
                          }
                      }
                      else {
                          CharacterParticleFactory::getInstance().generateTextParticle(playerX, playerY, "Missed", DamageTypes::DAMAGETYPE_NONE);
                      }
                      PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

                      MessageFactory factory;
                      std::stringstream ss;
                      ss << currentPlayerHealth;
                      GlobalConfigurations::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, 0.0,
                          PlayerFields::HEALTH, player.first, std::move(ss)
                      ).serialize().c_str());


                  }
              }
              else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                  if (SquareVsSquare(targetPosX, targetPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), playerX, playerY, 1.0f)) {
                      int currentPlayerHealth = player.second.getHealth();
                      std::vector<int> attackersModifiers = { 2 };
                      if (doesAttackLand(attackersModifiers, player.second.getAC())) {
                          std::vector<std::tuple<int, DamageTypes>> incomingDamage;
                          incomingDamage = calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());
                          for (std::tuple<int, DamageTypes> damage : incomingDamage) {
                              int tmpDamage = std::get<0>(damage);
                              CharacterParticleFactory::getInstance().generateTextParticle(playerX, playerY, "-" + std::to_string(tmpDamage), std::get<1>(damage));
                              currentPlayerHealth -= tmpDamage;
                          }
                      }
                      else {
                          CharacterParticleFactory::getInstance().generateTextParticle(playerX, playerY, "Missed", DamageTypes::DAMAGETYPE_NONE);
                      }

                      PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

                      MessageFactory factory;
                      std::stringstream ss;
                      ss << currentPlayerHealth;
                      GlobalConfigurations::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, 0.0,
                          PlayerFields::HEALTH, player.first, std::move(ss)
                      ).serialize().c_str());





                  }
              }
          }
      }
  }

  ~Greatclub(){};
};

#endif