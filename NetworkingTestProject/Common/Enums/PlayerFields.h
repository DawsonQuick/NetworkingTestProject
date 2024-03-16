#pragma once
#include <string>
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