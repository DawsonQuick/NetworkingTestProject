#pragma once
#ifndef FIREBALL_H
#define FIREBALL_H
#include "./../Particles/GuidedParticle.h"
#include "./Spell.h"
#include <iostream>
class Fireball : public Spell {
private:
public:
    void print() override {
        std::cout << "Hello from Fireball class" << std::endl;
    }
    void castSpell(float targetPosX, float targetPosY) override {
        //TODO: add a call to the specified Particle
    }
    ~Fireball() {};
};
#endif