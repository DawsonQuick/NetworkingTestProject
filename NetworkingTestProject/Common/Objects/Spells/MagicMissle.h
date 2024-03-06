#pragma once
#ifndef MAGICMISSLE_H
#define MAGICMISSLE_H
#include "./Spell.h"
#include "./../Particles/ParticleFactory.h"
#include <iostream>
class MagicMissle : public Spell {
private:
    float maxNumberofShots = 3.0f;

    //Number of grid cells
    float impactRadius = 0.0f;

    //Range is 120ft according to handbook 
    //Range / (ft per grid cell) = number of cells
    float range = 120.0f / (5.0f);

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
    void castSpell(float targetPosX , float targetPosY) override {
        ParticleDatabase::getInstance().addParticle("MagicMissle", ParticleFactory::getInstance().createGuidedParticle(PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(),
            targetPosX, targetPosY, 1.0f, [this](float resultPosX, float resultPoxY) { this->onComplete(resultPosX,resultPoxY); }));
    }

    void onComplete(float resultPosX, float resultPosY) {
        std::cout << "MagicMissle impacted at X: " << resultPosX << " Y: " << resultPosY << std::endl;

        //Example of chaining particles
        for (int i = 0; i < 100; i++) {ParticleDatabase::getInstance().addParticle("Test" + std::to_string(i), ParticleFactory::getInstance().createImplosionParticle(resultPosX, resultPosY,0.3f));}

        //Sample hit detection
        for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
            float playerX = player.second.getPositionX();
            float playerY = player.second.getPositionY();

            if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                //Circle:   PosX        PosY           Circle Radius # of Grid cells * Scale of cells      Player : Bottom-Left vertex position , Top-Right vertex position
                if (RectangleVsCircle(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), (playerX - 0.5f), (playerY - 0.5f), (playerX + 0.5f), (playerY + 0.5f))) {
                    std::cout << "Player: " << player.first << " hit with magic missle" << std::endl;
                    int currentPlayerHealth = player.second.getHealth();
                    currentPlayerHealth -= 5.0; //Imaginary MagicMissleDamage
                    PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

                }
            }
            else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                if (SquareVsSquare(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), playerX, playerY, 1.0f)) {
                    std::cout << "Player: " << player.first << " hit with magic missle" << std::endl;
                    int currentPlayerHealth = player.second.getHealth();
                    currentPlayerHealth -= 5.0; //Imaginary MagicMissleDamage
                    PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);
                }
            }
        }




    }
    ~MagicMissle() {};
};
#endif