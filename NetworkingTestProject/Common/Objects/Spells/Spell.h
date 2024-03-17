#pragma once
#ifndef SPELL_H
#define SPELL_H
#include "./../../Utils/CollsionCalculations/CollisionDetection.h"
#include "DurationSpellManager.h"
#include "./../../Enums/SpellType.h"
#include "./../../Messages/Utils/MessageFactory.h"
#include "./../../Enums/AbilityType.h"
#include "./../../Enums/Dice.h"
#include "./../../Enums/DamageTypes.h"
#include "./../../Utils/DamageCalcuator/DamageCalculator.h"

#include <iostream>

class Spell {
public:
    virtual std::string getName() const = 0;
    virtual void castSpell(std::string playerName,float targetPosX, float targetPosY) {};
    virtual float getImpactRadius() const = 0;
    virtual float getRange() const = 0;
    virtual float getMaxNumberofShots() const = 0;
    virtual SpellType getSpellType() const = 0;
    virtual int getSpellLevel() const = 0;
    virtual int getSpellDuration() const = 0;
    virtual std::function<void(float, float, std::string)> getCallback() { return nullptr;  };
    virtual ~Spell() {};

    Spell() {

    }
    // Define move constructor
    Spell(Spell&& other) noexcept {
        // Implement move semantics here
    }

    // Define move assignment operator
    Spell& operator=(Spell&& other) noexcept {
        if (this != &other) {
            // Implement move semantics here
        }
        return *this;
    }
};
#endif