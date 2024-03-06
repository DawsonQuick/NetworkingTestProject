#pragma once
#ifndef SPELL_H
#define SPELL_H
#include "./../../Utils/CollsionCalculations/CollisionDetection.h"

#include <iostream>
class Spell {
public:
    virtual std::string getName() const = 0;
    virtual void castSpell(float targetPosX, float targetPosY) {};
    virtual float getImpactRadius() const = 0;
    virtual float getRange() const = 0;
    virtual float getMaxNumberofShots() const = 0;
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