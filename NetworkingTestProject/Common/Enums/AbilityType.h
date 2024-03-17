#pragma once
#include <string>
enum AbilityType {
	STRENGTH,
	DEXTERITY,
	INTELLIGENCE,
	WISDOM,
	CHARISMA,
};

// Function to convert enum value to string
std::string playerFieldEnumToString(AbilityType value) {
	switch (value) {
	case AbilityType::STRENGTH: return "STRENGTH";
	case AbilityType::DEXTERITY: return "DEXTERITY";
	case AbilityType::INTELLIGENCE: return "INTELLIGENCE";
	case AbilityType::WISDOM: return "WISDOM";
	case AbilityType::CHARISMA: return "CHARISMA";

	}
}
