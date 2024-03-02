#pragma once
#ifndef BREASTPLATE_H
#define BREASTPLATE_H
#include "./Armour.h"
#include <iostream>
#include <memory>
#include <vector>
class BreastPlate : public Armour {
private:
    std::vector<std::shared_ptr<Spell>> spells;
public:
  BreastPlate(std::vector<std::shared_ptr<Spell>> tmpSpells) {
    spells = tmpSpells;
  }
  void print() override {
    std::cout << "Hello from Breast Plate class" << std::endl;
  }
  bool hasSpells() const override { return (!spells.empty()); };
  std::vector<std::shared_ptr<Spell>> getSpells() const override {
    return spells;
  };
  BreastPlate() {

  }
  ~BreastPlate(){};
};
#endif