#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <iostream>
#include <sstream>
#include <list>
struct Position {
	double X;
	double Y;
	double Z;

	//Populated constructor
	Position(double x, double y, double z)
		:X(x), Y(y), Z(z) {};

	//Empty constructor
	Position():X(0.0),Y(0.0),Z(0.0) {};
};
enum PlayerFields {
	NAME,
	POSITION,
	HEALTH,
	AC
};

// Function to convert enum value to string
std::string playerFieldEnumToString(PlayerFields value) {
	switch (value) {
	case PlayerFields::NAME: return "NAME";
	case PlayerFields::POSITION: return "POSITION";
	case PlayerFields::HEALTH: return "HEALTH";
	case PlayerFields::AC: return "AC";
	}
}

class Player {
private:
	
	std::string name;
	Position position;
	int health;
	int AC;

public:

	//Constructor
	Player(std::string name,double posX, double posY, double posZ, int health,int AC)
	: name(name),
	  health(health),
	  position(posX,posY,posZ),AC(AC)
	{

	}
	//Default Constructor
	Player()
	: name("Frodo"),
	  health(5),
	  position(0.0,0.0,0.0),AC(10) {}


/*
* -------------------------SETTERS--------------------------------
*/
	void setName(std::string name) {
		this->name = name;
	}
	void setHealth(int health) {
		this->health = health;
	}
	void setPosition(double posX,double posY, double posZ) {
		this->position.X = posX;
		this->position.Y = posY;
		this->position.Z = posZ;
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

/*
* -------------------------GETTERS--------------------------------
*/
	std::string getName() {
		return name;
	}
	int getHealth() {
		return health;
	}
	Position setPosition() {
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
		ss << name << "|" << position.X << "|" <<position.Y << "|" <<position.Z<<"|"<<health<<"|"<<AC;
		return ss.str();
	}

	static Player deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		std::string name;
		double posX, posY, posZ;
		int health,AC;
		char delimiter='|';
		std::getline(ss, name, delimiter);
		ss >> posX >> delimiter >> posY >> delimiter >> posZ >> delimiter >> health>>delimiter>>AC;
		return Player(name,posX,posY,posZ,health,AC);
	}

};
#endif