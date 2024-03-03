#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <chrono>
#include "./Spells/Spell.h"

struct Position {
	double X;
	double Y;
	double Z;
	long long pointTime;
	//Populated constructor
	Position(double x, double y, double z, long long pointTime)
		:X(x), Y(y), Z(z), pointTime(pointTime) {};

	//Empty constructor
	Position():X(0.0),Y(0.0),Z(0.0),pointTime(getCurrentTimeInMillis()) {};
};

struct KeyPress {
	bool keyW;
	bool keyA;
	bool keyS;
	bool keyD;
	KeyPress() : keyW(false), keyA(false), keyS(false), keyD(false) {
	};
	KeyPress(bool W, bool A, bool S, bool D) :keyW(W), keyA(A), keyS(S), keyD(D) {
	};

	// Define the != operator for KeyPress
	bool operator!=(const KeyPress& other) const {
		return (keyW != other.keyW) || (keyA != other.keyA) || (keyS != other.keyS) || (keyD != other.keyD);
	}

};
enum PlayerFields {
	NAME,
	POSITION,
	HEALTH,
	AC,
	KEYPRESS,
	MOVEMENTSPEED,
};

// Function to convert enum value to string
std::string playerFieldEnumToString(PlayerFields value) {
	switch (value) {
	case PlayerFields::NAME: return "NAME";
	case PlayerFields::POSITION: return "POSITION";
	case PlayerFields::HEALTH: return "HEALTH";
	case PlayerFields::AC: return "AC";
	case PlayerFields::KEYPRESS: return "KEYPRESS";
	case PlayerFields::MOVEMENTSPEED: return "MOVEMENTSPEED";
	}
}

class Player {
private:
	std::vector<std::shared_ptr<Spell>> spells;
	std::string name;
	Position position;
	KeyPress keyMap;
	int health;
	int AC;
	double movementSpeed;

public:

	//Constructor
	Player(std::string name,double posX, double posY, double posZ, int health,int AC)
	: name(name),
	  health(health),
	  position(posX,posY,posZ, getCurrentTimeInMillis()),AC(AC),
	  movementSpeed(0.01)
	{

	}


	//Default Constructor
	Player()
	: name("Frodo"),
	  health(5),
	  position(0.0,0.0,0.0, getCurrentTimeInMillis()),AC(10), movementSpeed(0.01) {}


	void addSpell(std::shared_ptr<Spell> spell) { spells.push_back(spell); }

/*
* -------------------------SETTERS--------------------------------
*/
	void setName(std::string name) {
		this->name = name;
	}
	void setHealth(int health) {
		this->health = health;
	}
	void setPosition(double posX,double posY, double posZ , long long tmpPointTime) {
		this->position.X = posX;
		this->position.Y = posY;
		this->position.Z = posZ;
		this->position.pointTime = tmpPointTime;
	}
	void setPositionX(double positionX) {
		this->position.X = positionX;
	}
	void setPositionY(double positionY) {
		this->position.Y = positionY;
	}
	void setPositionZ(double positionZ) {
		this->position.Z = positionZ;
	}
	void setAC(int AC) {
		this->AC = AC;
	}
	void setKeyPress(KeyPress tmpKeyMap) {
		keyMap = tmpKeyMap;
	}
	void setMovementSpeed(double tmpSpeed) {
		movementSpeed = tmpSpeed;
	}

/*
* -------------------------GETTERS--------------------------------
*/
	std::string getName() {
		return name;
	}
	int getHealth() {
		return health;
	}
	Position getPosition() {
		return this->position;
	}
	double getPositionX() {
		return this->position.X;
	}
	double getPositionY() {
		return this->position.Y;
	}
	double getPositionZ() {
		return this->position.Z;
	}
	int getAC() {
		return AC;
	}
	KeyPress getKeyPress() {
		return keyMap;
	}
	double getMovementSpeed() {
		return movementSpeed;
	}
	std::vector<std::shared_ptr<Spell>> getAvailableSpells() {
		return spells;
	}

/*
* ----------------------DISPLAY STATS------------------------------
*/
	std::string toString() {
		std::ostringstream builder;
		builder << " Name: " << name << " \n"
			<< " Position X: " << position.X << " \n"
			<< " Position Y: " << position.Y << " \n"
			<< " Position Z: " << position.Z << " \n"
			<< " Health: " << health << " \n"
			<< " AC: " << AC << "\n";
		return builder.str();
	}

/*
* ----------------------SERIZALIZE---------------------------------
*/
	std::string serialize() const {
		std::stringstream ss;
		ss << name << "|" << position.X << "|" <<position.Y << "|" <<position.Z<< "|" << health << "|" << AC;
		return ss.str();
	}

	static Player deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		std::string name;
		double posX, posY, posZ;
		int health,AC;
		char delimiter='|';
		std::getline(ss, name, delimiter);
		ss >> posX >> delimiter >> posY >> delimiter >> posZ>>delimiter >> health>>delimiter>>AC;
		return Player(name,posX,posY,posZ,health,AC);
	}

};
#endif