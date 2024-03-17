#pragma once
#include <string>
#include <random>
#include <chrono>
enum DiceType {
	D4,
	D6,
	D8,
	D10,
	D12,
	D20,
	DICETYPE_NONE,
};

// Function to convert enum value to string
std::string playerFieldEnumToString(DiceType value) {
	switch (value) {
	case DiceType::D4: return "D4";
	case DiceType::D6: return "D6";
	case DiceType::D8: return "D8";
	case DiceType::D10: return "D10";
	case DiceType::D12: return "D12";
	case DiceType::D20: return "D20";
	case DiceType::DICETYPE_NONE: return "DICETYPE_NONE";

	}
}

int rollDice(int numOfRolls , DiceType type) {
	int totalValue = 0;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis;
	for (int i = 0; i < numOfRolls; i++) {
		switch (type) {
		case DiceType::D4:
			dis = std::uniform_int_distribution<>(1, 4);
			totalValue += dis(gen);
			break;
		case DiceType::D6:
			dis = std::uniform_int_distribution<>(1, 6);
			totalValue += dis(gen);
			break;
		case DiceType::D8:
			dis = std::uniform_int_distribution<>(1, 8);
			totalValue += dis(gen);
			break;
		case DiceType::D10:
			dis = std::uniform_int_distribution<>(1, 10);
			totalValue += dis(gen);
			break;
		case DiceType::D12:
			dis = std::uniform_int_distribution<>(1, 12);
			totalValue += dis(gen);
			break;
		case DiceType::D20:
			dis = std::uniform_int_distribution<>(1, 20);
			totalValue += dis(gen);
			break;
		}
	}
	return totalValue;

}
