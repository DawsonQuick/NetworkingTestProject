//Message.h
#pragma once
#ifndef CASTSPELLMESSAGE_H
#define CASTSPELLMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"
#include <chrono>



class CastSpellMessage : public Message {

private:

	/*
	* -----------------HEADER---------------------------
	*/
	/*
	* Message Type
	*/
	MessageType type;

	/*
	* The unique identifier of the sender
	*/
	int UID;

	/*
	* Creation time of the message
	*/
	double creationTime;

	/*
	* -----------------CONTENT--------------------------
	*/

	/*
	* Player to update
	*/
	std::string name;

	float numberOfShots;

	/*
	* The stringstream representation of the updated data
	*/
	std::stringstream dataSS;

	/*
	* Time of update message creation
	*/
	long long pointTime;


public:
	/*
	* Constructor
	*/
	CastSpellMessage(MessageType tempType, int tmpUID, double tmpcreationTime,std::string tmpName, float tmpNumOfShots, std::stringstream&& tmpSS) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		name = tmpName;
		numberOfShots = tmpNumOfShots;
		pointTime = getCurrentTimeInMillis();
		dataSS = std::move(tmpSS);
	}


	/*
	* ----------------GETTERS----------------------------
	*/
	std::string getName() {
		return name;
	}

	std::stringstream getStringStream() {
		return std::move(dataSS);
	}

	float getNumberOfShots() {
		return numberOfShots;
	}

	/*
	* --------------Print MSG to Screen----------------
	*/

	void toString() {
		std::cout << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " Player name: " << name << "\n"
			<< " stringstream field: " << dataSS.str() << std::endl;
	}

	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime << "|" << pointTime << "|" << name << "|" << numberOfShots << "|";
		ss << dataSS.rdbuf();
		return ss.str();
	}


	// Deserialization method
	static CastSpellMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		std::string tempdataSS;
		std::string name;
		int enumID, fieldTypeID;
		int UID;
		long long pointTime;
		double creationTime;
		int commandInt;
		char delimiter;
		float numberofShots;
		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime >> delimiter >> pointTime >> delimiter;
		std::getline(ss, name, delimiter);
		ss >> numberofShots >> delimiter;
		ss >> tempdataSS;

		std::stringstream tempSS(tempdataSS);
		MessageType type = static_cast<MessageType>(enumID);
		return new CastSpellMessage(type, UID, creationTime, name, numberofShots, std::move(tempSS));
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif