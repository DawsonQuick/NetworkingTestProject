//Message.h
#pragma once
#ifndef CANCELCONCENTRATIONMESSAGE_H
#define CANCELCONCENTRATIONMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"
#include <chrono>



class CancelConcentrationMessage : public Message {

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


public:
	/*
	* Constructor
	*/
	CancelConcentrationMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpName) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		name = tmpName;
	}


	/*
	* ----------------GETTERS----------------------------
	*/
	std::string getName() {
		return name;
	}


	/*
	* --------------Print MSG to Screen----------------
	*/

	void toString() {
		std::cout << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " Player name: " << name << "\n";
	}

	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime <<"|" << name << "|";
		return ss.str();
	}


	// Deserialization method
	static CancelConcentrationMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		std::string name;
		int enumID;
		int UID;
		double creationTime;
		char delimiter;
		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime >> delimiter;
		std::getline(ss, name, delimiter);
		MessageType type = static_cast<MessageType>(enumID);
		return new CancelConcentrationMessage(type, UID, creationTime, name);
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif