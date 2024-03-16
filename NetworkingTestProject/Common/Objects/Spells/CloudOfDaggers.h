#pragma once
#ifndef CLOULDOFDAGGERS_H
#define CLOULDOFDAGGERS_H
#include "./Spell.h"
#include "./../Particles/ParticleFactory.h"
#include <iostream>
#include <limits>
class CloudOfDaggers : public Spell {
private:

    //All spells should include this data
    SpellType spellType = SpellType::CONCENTRATION;
    int spellLevel = 2;
    //Number of shots per turn
    float maxNumberofShots = 1.0f;
    //Number of grid cells
    float impactRadius = 5.0f / 5.0f;
    //Range is 120ft according to handbook 
    //Range / (ft per grid cell) = number of cells
    float range = 60.0f / (5.0f);




    //Only used for concentration or duration style spells
    float duration = 60000; //Spell last for 60 seconds (1 minute) if conentration is not broken or canceled , convert to millseconds
    float originX;  //The point at which the spell was targeted
    float originY;  //The point at which the spell was targeted

    float updateRate = 1000; //Purely used for rendering particles 

    std::string playerName;

public:
    std::string getName() const override {
        return "CloudOfDaggers";
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
        return duration;
    }
    std::function<void(float, float, std::string)> getCallback() override {
        return [this](float targetPosX, float targetPosY, std::string playerName) { this->onUpdate(targetPosX, targetPosY, playerName); };
    }

    void castSpell(std::string tmpPlayerName, float targetPosX, float targetPosY) override {

        float sideLength = (impactRadius * (GlobalConfigurations::getInstance().getScale()) + (GlobalConfigurations::getInstance().getScale() / 2.0f));
        DurationSpellManager::getInstance().addSpell(tmpPlayerName,getName(), spellType ,duration, updateRate, sideLength, glm::vec2(targetPosX, targetPosY), [this](float targetPosX, float targetPosY,std::string playerName) { this->onUpdate(targetPosX, targetPosY,playerName); });

    }

    void onUpdate(float originX,float originY, std::string playerName) {

        //Crude way of checking if concentration spell has expired
        //If Spell completes naturally by running out of time, need to cancel spell
        //If Spell was cancelled externally (CON spell override or break of CON) then this shouldnt trigger
        //Since the spell was removed from the DurationSpellManager
        if (originX == std::numeric_limits<float>::max() && originX == std::numeric_limits<float>::max()) {
            PlayerDatabase::getInstance().getPlayer(playerName).cancelConcentration();
        }

        //Example of chaining particles
        for (int i = 0; i < 50; i++) { ParticleDatabase::getInstance().addParticle("Test" + std::to_string(i), ParticleFactory::getInstance().createCloudOfDaggersParticle(originX, originY,impactRadius, updateRate, 0.3f)); }

    }
    ~CloudOfDaggers() {};
};
#endif