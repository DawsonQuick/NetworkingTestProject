#pragma once
#include <string>
#include "./../../OpenGL/vendor/glm/glm.hpp"
enum DamageTypes {
	DAMAGETYPE_NONE,
	ACID,
	BLUDGEONING,
	COLD,
	FIRE,
	FORCE,
	LIGHTNING,
	NECROTIC,
	PIERCING,
	POISON,
	PSYCHIC,
	RADIANT,
	SLASHING,
	THUNDER,

};

// Define variants for damage interactions
enum DamageInteraction {
	Resistance,
	Immunity,
	Vulnerability
};

// Function to convert enum value to color, used for text damage display
glm::vec4 playerFieDamageTypeEnumToColor(DamageTypes value) {
	switch (value) {
	case DamageTypes::ACID: return glm::vec4(40.0f / 255.0f, 252.0f / 255.0f, 3.0f / 255.0f, 255.0f / 255); break;
	case DamageTypes::BLUDGEONING: return glm::vec4(0.0f / 255.0f, 52.0f / 255.0f, 105.0f / 255.0f, 255.0f / 255); break;
	case DamageTypes::COLD: return glm::vec4(92.0f / 255.0f, 247.0f / 255.0f, 232.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::FIRE: return glm::vec4(235.0f / 255.0f, 49.0f / 255.0f, 2.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::FORCE: return glm::vec4(245.0f / 255.0f, 7.0f / 255.0f, 7.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::LIGHTNING: return glm::vec4(237.0f / 255.0f, 245.0f / 255.0f, 7.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::NECROTIC: return glm::vec4(71.0f / 255.0f, 4.0f / 255.0f, 179.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::PIERCING: return glm::vec4(142.0f / 255.0f, 159.0f / 255.0f, 184.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::POISON: return glm::vec4(109.0f / 255.0f, 247.0f / 255.0f, 162.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::PSYCHIC: return glm::vec4(252.0f / 255.0f, 71.0f / 255.0f, 246.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::RADIANT: return glm::vec4(252.0f / 255.0f, 236.0f / 255.0f, 3.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::SLASHING: return glm::vec4(252.0f / 255.0f, 102.0f / 255.0f, 3.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::THUNDER: return glm::vec4(82.0f / 255.0f, 122.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); break;
	case DamageTypes::DAMAGETYPE_NONE: return glm::vec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f); break;

	}
}

// Function to convert enum value to string
std::string DamageTypeEnumToString(DamageTypes value) {
	switch (value) {
	case DamageTypes::ACID: return "ACID";
	case DamageTypes::BLUDGEONING: return "BLUDGEONING";
	case DamageTypes::COLD: return "COLD";
	case DamageTypes::FIRE: return "FIRE";
	case DamageTypes::FORCE: return "FORCE";
	case DamageTypes::LIGHTNING: return "LIGHTNING";
	case DamageTypes::NECROTIC: return "NECROTIC";
	case DamageTypes::PIERCING: return "PIERCING";
	case DamageTypes::POISON: return "POISON";
	case DamageTypes::PSYCHIC: return "PSYCHIC";
	case DamageTypes::RADIANT: return "RADIANT";
	case DamageTypes::SLASHING: return "SLASHING";
	case DamageTypes::THUNDER: return "THUNDER";
	case DamageTypes::DAMAGETYPE_NONE: return "DAMAGETYPE_NONE";

	}
}

// Function to convert DamageInteraction enum value to string
std::string damageInteractionToString(DamageInteraction interaction) {
	switch (interaction) {
	case DamageInteraction::Resistance:    return "Resistance";
	case DamageInteraction::Immunity:      return "Immunity";
	case DamageInteraction::Vulnerability: return "Vulnerability";
	}
}
