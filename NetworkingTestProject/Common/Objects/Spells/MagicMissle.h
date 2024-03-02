#pragma once
#ifndef MAGICMISSLE_H
#define MAGICMISSLE_H
#include "./../Particles/GuidedParticle.h"
#include "./Spell.h"
#include <iostream>
class MagicMissle : public Spell {
private:
    bool RectangleVsCircle(float circlex, float circley, float circleRadius,
        float rect1x, float rect1y, float rect2x, float rect2y) {
        float distX = std::abs(circlex - ((rect2x + rect1x) / 2.0));
        float distY = std::abs(circley - ((rect2y + rect1y) / 2.0));

        if (distX > ((rect2x - rect1x) / 2.0) + circleRadius) {
            return false;
        }
        if (distY > ((rect2y - rect1y) / 2.0) + circleRadius) {
            return false;
        }

        if (distX <= ((rect2x - rect1x) / 2.0)) {
            return true;
        }
        if (distY <= ((rect2y - rect1y) / 2.0)) {
            return true;
        }

        float dx = distX - ((rect2x - rect1x) / 2.0);
        float dy = distY - ((rect2y - rect1y) / 2.0);
        return (dx * dx + dy * dy<= ((circleRadius * circleRadius)*GlobalConfigurations::getInstance().getScale()));
    }
public:
    void print() override {
        std::cout << "Hello from MagicMissle class" << std::endl;
    }
    void castSpell(float targetPosX , float targetPosY) override {
        //TODO: add a call to the specified Particle
        std::unique_ptr<Particle> particle = std::make_unique<GuidedParticle>(PlayerDatabase::getInstance().getPlayer(playerName).getPositionX(), PlayerDatabase::getInstance().getPlayer(playerName).getPositionY(),
            targetPosX, targetPosY, 1.0f, [this](float resultPosX, float resultPoxY) { this->onComplete(resultPosX,resultPoxY); });
        ParticalDatabase::getInstance().addParticle("MagicMissle", std::move(particle));
    }

    void onComplete(float resultPosX, float resultPosY) {
        std::cout << "MagicMissle impacted at X: " << resultPosX << " Y: " << resultPosY << std::endl;


        //Sample hit detection
        for (auto& player : PlayerDatabase::getInstance().getPlayers()) {
            float playerX = player.second.getPositionX();
            float playerY = player.second.getPositionY();
                        //Circle:   PosX        PosY            # of Grid cells * Scale of cells                        Player : Bottom-Left vertex position , Top-Right vertex position
            if (RectangleVsCircle(resultPosX, resultPosY, 2.0f * (GlobalConfigurations::getInstance().getScale()), (playerX - 0.5f), (playerY - 0.5f), (playerX + 0.5f), (playerY + 0.5f))) {

                std::cout << "Player: " << player.first << " hit with magic missle" << std::endl;
                int currentPlayerHealth = player.second.getHealth();
                currentPlayerHealth -= 5.0; //Imaginary MagicMissleDamage
                PlayerDatabase::getInstance().getPlayer(player.first).setHealth(currentPlayerHealth);

            };

        }




    }
    ~MagicMissle() {};
};
#endif