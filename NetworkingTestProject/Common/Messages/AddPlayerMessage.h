//Message.h
#ifndef PLAYERMESSAGE_H
#define PLAYERMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"

class AddPlayerMessage : public Message {

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
	* Payload
	*/
	std::stringstream player;


public:
	/*
	* Constructor
	*/
	AddPlayerMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::stringstream tmpPlayer) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		player = std::move(tmpPlayer);
	}

	/*
	* ----------------GETTERS----------------------------
	*/
	std::stringstream getPlayer() {
		return std::move(player);
	}
	/*
	* --------------Print MSG to Screen----------------
	*/

	std::string toString() {
		std::ostringstream builder;
		builder << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " Player : " << "\n";
		return builder.str();
	}
	/*k
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime << "|" << player.rdbuf()<<"|";
		return ss.str();
	}


	// Deserialization method
	static AddPlayerMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		int enumID;
		int UID;
		std::string playerSerialized;
		double creationTime;
		int commandInt;
		char delimiter;

		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime >> delimiter >> playerSerialized>>delimiter;
		std::stringstream tmpPlayerSS(playerSerialized);
		MessageType type = static_cast<MessageType>(enumID);
		return new AddPlayerMessage(type, UID, creationTime, std::move(tmpPlayerSS));
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif