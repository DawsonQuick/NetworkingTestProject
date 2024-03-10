#pragma once
#include <string>
enum SpellType {
    INSTANTANEOUS,
    DURATION,
    CONCENTRATION
};

// Function to convert enum value to string
std::string spellTypeToString(SpellType value) {
    switch (value) {
    case SpellType::INSTANTANEOUS: return "INSTANTANEOUS";
    case SpellType::DURATION: return "DURATION";
    case SpellType::CONCENTRATION: return "CONCENTRATION";
    }
}