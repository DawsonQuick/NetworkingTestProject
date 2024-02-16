//Message.h
#ifndef HANDSHAKEMESSAGE_H
#define HANDSHAKEMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"

MessageType msgConversion;
class HandShakeMessage : public Message {

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


public:
	/*
	* Constructor
	*/
	HandShakeMessage(MessageType tempType ,int tmpUID, double tmpcreationTime) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
	}

/*
* ----------------GETTERS----------------------------
*/


/*
* --------------Print MSG to Screen----------------
*/

	std::string toString() {
		std::ostringstream builder;
		builder << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n";
		return builder.str();
	}
/*
* ----------------------------------Message transportation packaging----------------------------------------------
*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime;
		return ss.str();
	}


	// Deserialization method
	static HandShakeMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		int enumID;
		int UID;
		double creationTime;
		int commandInt;
		char delimiter;

		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime;
		MessageType type = static_cast<MessageType>(enumID);
		return new HandShakeMessage(type,UID, creationTime);
	}
/*
* ---------------------------------------------------------------------------------------------------------------
*/
};

#endif