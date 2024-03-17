#pragma once
#ifndef MAGICMISSLE_H
#define MAGICMISSLE_H
#include "./Spell.h"
#include "./../Particles/ParticleFactory.h"
#include <iostream>
class MagicMissle : public Spell {
private:
    SpellType spellType = SpellType::INSTANTANEOUS;
    int spellLevel = 1; 
    float maxNumberofShots = 3.0f;

    //Number of grid cells
    float impactRadius = 0.0f;

    //Per PlayerHandbook , Damage = 1d4 + 1 Force Damage
    std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
        {1,DiceType::D4,DamageTypes::DAMAGETYPE_NONE},
        {1,DiceType::DICETYPE_NONE,DamageTypes::FORCE},
    };

    //Range is 120ft according to handbook 
    //Range / (ft per grid cell) = number of cells
    float range = 120.0f / (5.0f);
    std::string playerName;
public:
    std::string getName() const override {
        return "MagicMissle";
    }
    float getImpactRadius() const override {
        return impactRadius;
    }
    float getRange() const override {
        return range;
    }
    float getMaxNumberofShots() const override {
        return maxNumberofShots;
    }
    SpellType getSpellType() const override {
        return spellType;
    }
    int getSpellLevel() const override {
        return spellLevel;
    }
    int getSpellDuration() const override {
        return 1000; //Spell is Instantaneous so just return 1 second
    }
    std::function<void(float, float, std::string)> getCallback() override{
        return nullptr;
    }

    void castSpell(std::string tmpPlayerName, float targetPosX , float targetPosY) override {
        playerName = tmpPlayerName;
        ParticleDatabase::getInstance().addParticle("MagicMissle", ParticleFactory::getInstance().createGuidedParticle(PlayerDatabase::getInstance().getPlayer(tmpPlayerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(tmpPlayerName).getPositionY(),
            targetPosX, targetPosY, 0.5f, [this](float resultPosX, float resultPoxY) { this->onComplete(resultPosX,resultPoxY); }));
    }

    void onComplete(float resultPosX, float resultPosY) {
        std::cout << "MagicMissle impacted at X: " << resultPosX << " Y: " << resultPosY << std::endl;

        //Example of chaining particles
        //for (int i = 0; i < 100; i++) {ParticleDatabase::getInstance().addParticle("Test" + std::to_string(i), ParticleFactory::getInstance().createImplosionParticle(resultPosX, resultPosY,0.3f));}
        if (playerName == GlobalConfigurations::getInstance().getPlayerName()) {
            //Sample hit detection
            for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
                float playerX = player.second.getPositionX();
                float playerY = player.second.getPositionY();

                if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                    //Circle:   PosX        PosY           Circle Radius # of Grid cells * Scale of cells      Player : Bottom-Left vertex position , Top-Right vertex position
                    if (RectangleVsCircle(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), (playerX - 0.5f), (playerY - 0.5f), (playerX + 0.5f), (playerY + 0.5f))) {
                        
                        //Magic Missle has a 100% chance to hit so no need to calculate if hit happens
                            int currentPlayerHealth = player.second.getHealth();
                            currentPlayerHealth -= calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());
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
                    if (SquareVsSquare(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), playerX, playerY, 1.0f)) {

                            //Magic Missle has a 100% chance to hit so no need to calculate if hit happens
                            int currentPlayerHealth = player.second.getHealth();
                            currentPlayerHealth -= calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());
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
    ~MagicMissle() {};
};
#endif