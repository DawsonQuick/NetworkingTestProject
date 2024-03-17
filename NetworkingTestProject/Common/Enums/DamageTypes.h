#pragma once
#include <string>
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

// Function to convert enum value to string
std::string playerFieldEnumToString(DamageTypes value) {
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
