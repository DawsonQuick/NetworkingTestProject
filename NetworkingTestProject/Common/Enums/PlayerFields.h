#pragma once
#include <string>
#include "./AbilityType.h"
enum PlayerFields {
	NAME,
	POSITION,
	HEALTH,
	AC,
	MOVEMENTSPEED,
	SELECTEDSPELL,
	SPELLCHANGESTATE,
	CURRENTSPELLSHOTCOUNT,
	ISCURRENTLYCONCENTRATING,
	CONCENTRATIONSPELL,
	CONCENTRATIONSPELLLOCATION,
	CONCENTRATIONROUNDSLEFT,
	ISTURNREADY,
	ISTURNCOMPLETE,
};

// Function to convert enum value to string
std::string playerFieldEnumToString(PlayerFields value) {
	switch (value) {
	case PlayerFields::NAME: return "NAME";
	case PlayerFields::POSITION: return "POSITION";
	case PlayerFields::HEALTH: return "HEALTH";
	case PlayerFields::AC: return "AC";
	case PlayerFields::MOVEMENTSPEED: return "MOVEMENTSPEED";
	case PlayerFields::SELECTEDSPELL: return "SELECTEDSPELL";
	case PlayerFields::SPELLCHANGESTATE: return "SPELLCHANGESTATE";
	case PlayerFields::CURRENTSPELLSHOTCOUNT: return "CURRENTSPELLSHOTCOUNT";
	case PlayerFields::ISCURRENTLYCONCENTRATING: return "ISCURRENTLYCONCENTRATING";
	case PlayerFields::CONCENTRATIONSPELL: return "CONCENTRATIONSPELL";
	case PlayerFields::CONCENTRATIONSPELLLOCATION: return "CONCENTRATIONSPELLLOCATION";
	case PlayerFields::CONCENTRATIONROUNDSLEFT: return "CONCENTRATIONROUNDSLEFT";
	case PlayerFields::ISTURNREADY: return "ISTURNREADY";
	case PlayerFields::ISTURNCOMPLETE: return "ISTURNCOMPLETE";
	}
}


struct Position {
	double X;
	double Y;
	double Z;
	long long pointTime;
	//Populated constructor
	Position(double x, double y, double z, long long pointTime)
		:X(x), Y(y), Z(z), pointTime(pointTime) {};

	//Empty constructor
	Position() :X(0.0), Y(0.0), Z(0.0), pointTime(0.0) {};
};

struct AbilityScore {
	int Strength;
	int Dexterity;
	int Intelligence;
	int Wisdom;
	int Charisma;

	int getModifier(AbilityType type) {

		int typeToCheck = -1;
		switch (type) {
		case AbilityType::STRENGTH:
			typeToCheck = Strength;
			break;
		case AbilityType::DEXTERITY:
			typeToCheck = Dexterity;
			break;
		case AbilityType::INTELLIGENCE:
			typeToCheck = Intelligence;
			break;
		case AbilityType::WISDOM:
			typeToCheck = Wisdom;
			break;
		case AbilityType::CHARISMA:
			typeToCheck = Charisma;
			break;
		}


		switch (typeToCheck) {
		case 1:
			return -5;
			break;
		case 2:
		case 3:
			return -4;
			break;
		case 4:
		case 5:
			return -3;
			break;
		case 6:
		case 7:
			return -2;
			break;
		case 8:
		case 9:
			return -1;
			break;
		case 10:
		case 11:
			return 0;
			break;
		case 12:
		case 13:
			return 1;
			break;
		case 14:
		case 15:
			return 2;
			break;
		case 16:
		case 17:
			return 3;
			break;
		case 18:
		case 19:
			return 4;
			break;
		case 20:
		case 21:
			return 5;
			break;
		case 22:
		case 23:
			return 6;
			break;
		case 24:
		case 25:
			return 7;
			break;
		case 26:
		case 27:
			return 8;
			break;
		case 28:
		case 29:
			return 9;
			break;
		case 30:
			return 10;
			break;
		}
	}

};
