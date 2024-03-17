#pragma once
#ifndef FIREBALL_H
#define FIREBALL_H
#include "./../Particles/GuidedParticle.h"
#include "./../Particles/ParticleFactory.h"
#include "./Spell.h"
#include <iostream>
class Fireball : public Spell {
private:
    SpellType spellType = SpellType::INSTANTANEOUS;
    
    float maxNumberofShots = 1;
    
    int spellLevel = 3;

    //Number of grid cells
    float impactRadius = 20.0f / 5.0f;

    //8d6 Fire Damage
    std::vector<std::tuple<int, DiceType, DamageTypes>> damageModifier = {
        {8,DiceType::D6,DamageTypes::FIRE},
    };

    //Range is 120ft according to handbook 
    //Range / (ft per grid cell) = number of cells
    float range = 150.0f / (5.0f);
    std::string playerName;
public:
    std::string getName() const override {
        return "Fireball";
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
    std::function<void(float, float, std::string)> getCallback() override {
        return nullptr;
    }
    void castSpell(std::string tmpPlayerName, float targetPosX, float targetPosY) override {
        playerName = tmpPlayerName;
        //TODO: Send a message when casting starts saying that this spell has been cast to start the partical animation on other players screens
        ParticleDatabase::getInstance().addParticle("Fireball", ParticleFactory::getInstance().createGuidedParticle(PlayerDatabase::getInstance().getPlayer(tmpPlayerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(tmpPlayerName).getPositionY(),
            targetPosX, targetPosY, 1.0f, [this](float resultPosX, float resultPoxY) { this->onComplete(resultPosX, resultPoxY); }));
    }

    void onComplete(float resultPosX, float resultPosY) {

        //TODO: Send another message when spell cast is finished letting other players know the resulting effects
        std::cout << "Fireball impacted at X: " << resultPosX << " Y: " << resultPosY << std::endl;

        for (int i = 0; i < 100; i++) { ParticleDatabase::getInstance().addParticle("Test" + std::to_string(i), ParticleFactory::getInstance().createTestParticle(resultPosX,resultPosY,0.0f,0.0f,0.2f)); }

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
                            int currentPlayerHealth = player.second.getHealth();
                            int IncomingDamage = 0;
                            IncomingDamage = calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());
                            
                            std::vector<int> tmpModifier = { 2 };
                            if (doesTargetSave(14, tmpModifier)) { IncomingDamage = IncomingDamage / 2;  std::cout << "Player: " << player.first << " passed the saving throw!" << std::endl;
                            }
                            currentPlayerHealth -= IncomingDamage;
                            
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
                            int currentPlayerHealth = player.second.getHealth();
                            int IncomingDamage = 0;
                            IncomingDamage = calculateDamage(damageModifier, player.second.getPlayerDamageAttributes());

                            std::vector<int> tmpModifier = { 2 };
                            if (doesTargetSave(14, tmpModifier)) { IncomingDamage = IncomingDamage / 2; std::cout << "Player: " << player.first << " passed the saving throw!" << std::endl; }
                            currentPlayerHealth -= IncomingDamage;
                            PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

                            MessageFactory factory;
                            std::stringstream ss;
                            ss << currentPlayerHealth;
                            GlobalConfigurations::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, 0.0,
                                PlayerFields::HEALTH, player.first, std::move(ss)
                            ).serialize().c_str());

                            //Add check for Saving throw to see if damage is halved

                    }
                }
            }
        }




    }
    ~Fireball() {};
};
#endif