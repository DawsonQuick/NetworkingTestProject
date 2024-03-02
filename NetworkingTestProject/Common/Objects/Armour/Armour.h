#pragma once
#include "./../Spells/Spell.h"
#include <iostream>
#include <memory>
#include <vector>
class Armour {
public:

    virtual void print() {
        std::cout << "Hello from Armour Parent class" << std::endl;
    }
    virtual bool hasSpells() const = 0 {};
    virtual std::vector<std::shared_ptr<Spell>> getSpells() const = 0 {} ;
    virtual ~Armour() {};
};

