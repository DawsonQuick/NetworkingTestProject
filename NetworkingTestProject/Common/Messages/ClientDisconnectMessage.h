//Message.h
#ifndef CLIENTDISCONNECTMESSAGE_H
#define CLIENTDISCONNECTMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"

class ClientDisconnectMessage : public Message {

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

	std::string name;


public:
	/*
	* Constructor
	*/
	ClientDisconnectMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpName) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		name = tmpName;
	}

	/*
	* ----------------GETTERS----------------------------
	*/
	std::string getPlayerName() {
		return name;
	}

	/*
	* --------------Print MSG to Screen----------------
	*/

	std::string toString() {
		std::ostringstream builder;
		builder << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " name: " << name << "\n";
		return builder.str();
	}
	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime <<"|"<<name;
		return ss.str();
	}


	// Deserialization method
	static ClientDisconnectMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		int enumID;
		int UID;
		double creationTime;
		int commandInt;
		char delimiter;
		std::string name;
		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime>> delimiter;
		std::getline(ss, name);
		MessageType type = static_cast<MessageType>(enumID);
		return new ClientDisconnectMessage(type, UID, creationTime, name);
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif