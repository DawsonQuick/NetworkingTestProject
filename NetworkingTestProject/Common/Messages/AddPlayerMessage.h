//Message.h
#ifndef PLAYERMESSAGE_H
#define PLAYERMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Objects/Player.h"
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
	Player player;


public:
	/*
	* Constructor
	*/
	AddPlayerMessage(MessageType tempType, int tmpUID, double tmpcreationTime, Player tmpPlayer) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		player = tmpPlayer;
	}

	/*
	* ----------------GETTERS----------------------------
	*/
	Player getPlayer() {
		return player;
	}
	/*
	* --------------Print MSG to Screen----------------
	*/

	std::string toString() {
		std::ostringstream builder;
		builder << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " Player : " << "\n"
			<< player.toString();
		return builder.str();
	}
	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() {
		player.toString();
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime << "|" << player.serialize()<<"|";
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
		Player player = Player::deserialize(playerSerialized.c_str());
		MessageType type = static_cast<MessageType>(enumID);
		return new AddPlayerMessage(type, UID, creationTime, player);
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif