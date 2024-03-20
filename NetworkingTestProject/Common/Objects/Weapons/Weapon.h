#pragma once
#ifndef WEAPON_H
#define WEAPON_H

#include "./../../Utils/CollsionCalculations/CollisionDetection.h"
#include "./../../Messages/Utils/MessageFactory.h"
#include "./../../Enums/AbilityType.h"
#include "./../../Enums/Dice.h"
#include "./../../Enums/DamageTypes.h"
#include "./../../Enums/WeaponType.h"
#include "./../../Utils/DamageCalcuator/DamageCalculator.h"
#include <cmath>
#include <iostream>
#include <functional>

class Weapon {
public:
    virtual std::string getName() const = 0;
    virtual void attack(std::string playerName, float targetPosX, float targetPosY) {};
    virtual float getImpactRadius() const = 0;
    virtual float getRange() const = 0;
    virtual WeaponType getWeaponType() const = 0;
    virtual std::vector<WeaponProperties> getWeaponProperties() const = 0;
    virtual ~Weapon() {};

    Weapon() {

    }
    // Define move constructor
    Weapon(Weapon&& other) noexcept {
        // Implement move semantics here
    }

    // Define move assignment operator
    Weapon& operator=(Weapon&& other) noexcept {
        if (this != &other) {
            // Implement move semantics here
        }
        return *this;
    }
};
#endif