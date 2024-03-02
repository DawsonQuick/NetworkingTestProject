#pragma once
#ifndef SPELL_H
#define SPELL_H

#include <iostream>
class Spell {
public:
    virtual void print() {
        std::cout << "Hello from Spell Parent class" << std::endl;
    }
    virtual void castSpell(float targetPosX, float targetPosY) {};
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