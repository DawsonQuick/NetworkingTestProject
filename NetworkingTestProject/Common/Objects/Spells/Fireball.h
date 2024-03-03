#pragma once
#ifndef FIREBALL_H
#define FIREBALL_H
#include "./../Particles/GuidedParticle.h"
#include "./Spell.h"
#include <iostream>
class Fireball : public Spell {
private:
    //Number of grid cells
    float impactRadius = 20.0f / 5.0f;

    //Range is 120ft according to handbook 
    //Range / (ft per grid cell) = number of cells
    float range = 150.0f / (5.0f);
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

    void castSpell(float targetPosX, float targetPosY) override {
        //TODO: Send a message when casting starts saying that this spell has been cast to start the partical animation on other players screens
        ParticleDatabase::getInstance().addParticle("Fireball", ParticleFactory::getInstance().createGuidedParticle(PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(),
            targetPosX, targetPosY, 1.0f, [this](float resultPosX, float resultPoxY) { this->onComplete(resultPosX, resultPoxY); }));
    }

    void onComplete(float resultPosX, float resultPosY) {

        //TODO: Send another message when spell cast is finished letting other players know the resulting effects
        std::cout << "Fireball impacted at X: " << resultPosX << " Y: " << resultPosY << std::endl;

        for (int i = 0; i < 100; i++) { ParticleDatabase::getInstance().addParticle("Test" + std::to_string(i), ParticleFactory::getInstance().createTestParticle(resultPosX,resultPosY,0.0f,0.0f,0.3f)); }
        //Sample hit detection
        for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
            float playerX = player.second.getPositionX();
            float playerY = player.second.getPositionY();

            if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GOEMETRIC) {
                //Circle:   PosX        PosY           Circle Radius # of Grid cells * Scale of cells      Player : Bottom-Left vertex position , Top-Right vertex position
                if (RectangleVsCircle(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), (playerX - 0.5f), (playerY - 0.5f), (playerX + 0.5f), (playerY + 0.5f))) {
                    std::cout << "Player: " << player.first << " hit with Fireball" << std::endl;
                    int currentPlayerHealth = player.second.getHealth();
                    currentPlayerHealth -= 5.0; //Imaginary MagicMissleDamage
                    PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

                }
            }
            else if (GlobalConfigurations::getInstance().getCurrentMeasurmentSystem() == MeasurmentSystem::GRID) {
                if (SquareVsSquare(resultPosX, resultPosY, impactRadius * (GlobalConfigurations::getInstance().getScale()), playerX, playerY, 1.0f)) {
                    std::cout << "Player: " << player.first << " hit with Fireball" << std::endl;
                    int currentPlayerHealth = player.second.getHealth();
                    currentPlayerHealth -= 5.0; //Imaginary MagicMissleDamage
                    PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);
                }
            }
        }




    }
    ~Fireball() {};
};
#endif