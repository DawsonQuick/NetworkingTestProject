#pragma once
#include "./../../Enums/AbilityType.h"
#include "./../../Enums/Dice.h"
#include "./../../Enums/DamageTypes.h"
#include <map>
#include <vector>
#include <tuple>
#include <cmath>

bool doesAttackLand(std::vector<int> attackersModifiers, int armourClassToBeat) {
	int initialAttackRoll = rollDice(1, DiceType::D20);
	for (int modifier : attackersModifiers) {
		initialAttackRoll += modifier;
	}

	return (initialAttackRoll >= armourClassToBeat);

}

bool doesTargetSave(int targetDcThrow, std::vector<int> modifiers) {
	int initalSaveRoll = rollDice(1, DiceType::D20);
	for (int modifier : modifiers) {
		initalSaveRoll += modifier;
	}

	return (initalSaveRoll >= targetDcThrow);
}

int calculateInterationModifier(int currentDamage, DamageTypes type, std::map<DamageTypes, DamageInteraction> playerDamageAttributes) {
	for (auto& playerAttribute : playerDamageAttributes) {

				if (type == playerAttribute.first) {
					switch (playerAttribute.second) {
					case DamageInteraction::Immunity:
						currentDamage = 0;
						break;	
					case DamageInteraction::Resistance:
						currentDamage = std::floor(currentDamage /2);
						break;	
					case DamageInteraction::Vulnerability:
						currentDamage = currentDamage * 2;
						break;
			}
		}
	}
	return currentDamage;
}


//Todo return a some sort of tuple/pair for damage and type instead of adding it all together here.
//That way the caller can then generate the Text particle for each type and that way you can tie a color 
//To the text based on the damage type
std::vector<std::tuple<int, DamageTypes>> calculateDamage(std::vector<std::tuple<int, DiceType, DamageTypes>> allIncomingDamage,
	std::map<DamageTypes, DamageInteraction> playerDamageAttributes) {
	std::vector<std::tuple<int, DamageTypes>> returnDamage;

	for (std::tuple<int, DiceType, DamageTypes> damageRoll : allIncomingDamage) {

		//If the DiceType is set to NONE, then the first entry in the tuple will be stright damage
		if (std::get<1>(damageRoll) == DiceType::DICETYPE_NONE) {
			int tmpDamage = 0;
			tmpDamage = calculateInterationModifier(std::get<0>(damageRoll), std::get<2>(damageRoll), playerDamageAttributes);

			std::tuple<int, DamageTypes> tmp = std::make_tuple(tmpDamage, std::get<2>(damageRoll));
			returnDamage.push_back(tmp);
			continue;
		}
		else {
			int tmpDamage = rollDice(std::get<0>(damageRoll), std::get<1>(damageRoll));
			if (std::get<2>(damageRoll) != DamageTypes::DAMAGETYPE_NONE) {
				tmpDamage = calculateInterationModifier(tmpDamage, std::get<2>(damageRoll), playerDamageAttributes);

			}
			std::tuple<int, DamageTypes> tmp = std::make_tuple(tmpDamage, std::get<2>(damageRoll));
			returnDamage.push_back(tmp);
		}
	}

	return returnDamage;

}
