//Message.h
#ifndef BROADCASTMESSAGE_H
#define BROADCASTMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"

class BroadCastMessage : public Message {

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
	std::string msg;


public:
	/*
	* Constructor
	*/
	BroadCastMessage(MessageType tempType ,int tmpUID, double tmpcreationTime, std::string tmpMsg) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		msg = tmpMsg;
	}

	/*
	* ----------------GETTERS----------------------------
	*/

	std::string getMsg() {
		return msg;
	}

	/*
	* --------------Print MSG to Screen----------------
	*/

	std::string toString() {
		std::ostringstream builder;
		builder << " UID: " << UID << " \n"
			<< "MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " msg: " << msg << " \n";
		return builder.str();
	}
	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss <<static_cast<int>(type)<<"|" << UID << "|" << creationTime << "|" << msg;
		return ss.str();
	}


	// Deserialization method
	static BroadCastMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		int enumID;
		int UID;
		double creationTime;
		std::string msg;
		int commandInt;
		char delimiter;

		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime >> delimiter;
		std::getline(ss,msg,delimiter);
		MessageType type = static_cast<MessageType>(enumID);
		return new BroadCastMessage(type,UID, creationTime,msg);
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};
#endif