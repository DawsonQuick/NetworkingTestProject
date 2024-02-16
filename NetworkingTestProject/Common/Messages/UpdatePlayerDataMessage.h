//Message.h
#ifndef UPDATEPLAYERDATAEMESSAGE_H
#define UPDATEPLAYERDATAMESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./Message.h"
#include "./../Enums/MessageTypes.h"
#include "./../Objects/Player.h"
class UpdatePlayerDataMessage : public Message {

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
	* Enum to determine which field needs to be updated
	*/
	PlayerFields fieldType;

	/*
	* Player to update
	*/
	std::string name;

	/*
	* The stringstream representation of the updated data
	*/
	std::stringstream dataSS;


public:
	/*
	* Constructor
	*/
	UpdatePlayerDataMessage(MessageType tempType, int tmpUID, double tmpcreationTime, PlayerFields field ,std::string tmpName, std::stringstream&& tmpSS) {
		type = tempType;
		UID = tmpUID;
		creationTime = tmpcreationTime;
		fieldType = field;
		name = tmpName;
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

	PlayerFields getField() {
		return fieldType;
	}


	/*
	* --------------Print MSG to Screen----------------
	*/

	void toString() {
		std::cout << " UID: " << UID << " \n"
			<< " MessageType: " << enumToString(type) << "\n"
			<< " creationTime: " << creationTime << " \n"
			<< " Player field: " << playerFieldEnumToString(fieldType) << "\n"
			<< " Player name: " << name << "\n"
			<< " stringstream field: " << dataSS.str() << std::endl;
	}	

	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
		std::stringstream ss;
		ss << static_cast<int>(type) << "|" << UID << "|" << creationTime << "|" << static_cast<int>(fieldType) << "|" << name << "|";
		ss << dataSS.rdbuf();
		return ss.str();
	}


	// Deserialization method
	static UpdatePlayerDataMessage* deserialize(const char* serializedData) {
		std::stringstream ss(serializedData);
		std::string tempdataSS;
		std::string name;
		int enumID , fieldTypeID;
		int UID;
		double creationTime;
		int commandInt;
		char delimiter;

		ss >> enumID >> delimiter >> UID >> delimiter >> creationTime >> delimiter >> fieldTypeID >> delimiter ;
		std::getline(ss, name, delimiter);
		ss>> tempdataSS;

		std::stringstream tempSS(tempdataSS);
		MessageType type = static_cast<MessageType>(enumID);
		PlayerFields fieldType = static_cast<PlayerFields>(fieldTypeID);
		return new UpdatePlayerDataMessage(type, UID, creationTime,fieldType, name,std::move(tempSS));
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};

#endif