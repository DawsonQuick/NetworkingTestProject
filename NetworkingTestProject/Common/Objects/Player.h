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
	int health;
	int AC;
	double movementSpeed;
	std::shared_ptr<Spell> selectedSpell;
	bool spellChangedState; 
	float currentSpellShotCount;

	bool isCurrentlyConcentrating;
	std::shared_ptr<Spell> concentrationSpell;
	int concentrationRoundsLeft;

	bool isTurnReady;
	bool isTurnComplete;

public:

	//Constructor
	Player(std::string name, double posX, double posY, double posZ, int health, int AC)
		: name(name),
		health(health),
		position(posX, posY, posZ, getCurrentTimeInMillis()), AC(AC),
		movementSpeed(0.01),
		spellChangedState(false)
	{

	}


	//Default Constructor
	Player()
	: name("Frodo"),
	  health(5),
	  position(0.0,0.0,0.0, getCurrentTimeInMillis()),AC(10), movementSpeed(0.01) {}

	~Player() {

	}


	void addSpell(std::shared_ptr<Spell> spell) { spells.push_back(spell); }

/*
* -------------------------SETTERS--------------------------------
*/
	void setIsTurnReady(bool readyState) {
		isTurnReady = readyState;
	}
	void setIsTurnComplete(bool completeState) {
		isTurnComplete = completeState;
	}
	void setName(std::string tmpName) {
		name = tmpName;
	}
	void setHealth(int tmpHealth) {
		health = tmpHealth;

	}
	void setPosition(double posX,double posY, double posZ , long long tmpPointTime) {
		position.X = posX;
		position.Y = posY;
		position.Z = posZ;
		position.pointTime = tmpPointTime;
	}
	void setPositionX(double positionX) {
		position.X = positionX;
	}
	void setPositionY(double positionY) {
		position.Y = positionY;
	}
	void setPositionZ(double positionZ) {
		position.Z = positionZ;
	}
	void setAC(int tmpAC) {
		AC = tmpAC;
	}
	void setMovementSpeed(double tmpSpeed) {
		movementSpeed = tmpSpeed;
	}
	void setSelectedSpell(std::shared_ptr<Spell> spell) {
		currentSpellShotCount = 1.0f; //reset shot count for change in spell
		spellChangedState = true;
		selectedSpell = spell;
	}
	void setCurrentSpellShotCount(float count) {
		currentSpellShotCount = count;
	}
	float getCurrentSpellShotCount() {
		return currentSpellShotCount;
	}
	void resetChangeState(bool changeState) {
		spellChangedState = changeState;
	}

	bool hasSpellChanged(){
		return spellChangedState;
	}


	std::shared_ptr<Spell> getSelectedSpell() {
		return selectedSpell;
	}
	void castSpell(float targetX, float targetY) {
		//Do upkeep spell tracking here before casting, if current casting spell is concentration , check to see if another concentration spell
		//is currently active , if so then need to stop that spell and replace with this one.

		if (selectedSpell->getSpellType() == SpellType::CONCENTRATION) {
			//Check to see if player currently has a active concentration spell
			if (isCurrentlyConcentrating) {
				//If currently concentrating and then cast another concentration spell, break the old connection and cast the new one
				DurationSpellManager::getInstance().removeConcentrationSpell(name, concentrationSpell->getName());
			}
			isCurrentlyConcentrating = true;
			concentrationRoundsLeft = selectedSpell->getSpellDuration()/6000; //Divide by the time taken per round
			concentrationSpell = selectedSpell;
		}

		
		selectedSpell->castSpell(name,targetX, targetY);
	}

	void cancelConcentration() {
		if (isCurrentlyConcentrating) {
			DurationSpellManager::getInstance().removeConcentrationSpell(name, concentrationSpell->getName());
			concentrationSpell.reset();
			concentrationRoundsLeft = 0;
			isCurrentlyConcentrating = false;
		}
	}

	bool isPlayerCurrentlyConcentrating() {
		return isCurrentlyConcentrating;
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
		return position;
	}
	double getPositionX() {
		return position.X;
	}
	double getPositionY() {
		return position.Y;
	}
	double getPositionZ() {
		return position.Z;
	}
	int getAC() {
		return AC;
	}
	double getMovementSpeed() {
		return movementSpeed;
	}
	std::vector<std::shared_ptr<Spell>> getAvailableSpells() {
		return spells;
	}

	bool getIsTurnReady() {
		return isTurnReady;
	}
	bool getIsTurnComplete() {
		return isTurnComplete;
	}
	std::string getCurrentConcentrationSpellName() {
		return concentrationSpell->getName();
	}

	void setRoundsRemainingForConcentrationSpell(int rounds) {
		concentrationRoundsLeft = rounds;
	}

	int getRoundsRemainingForConcentrationSpell() {
		return concentrationRoundsLeft;
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