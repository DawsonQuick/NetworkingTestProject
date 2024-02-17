//Message.h
#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <iostream>
#include <sstream>
#include "./../Enums/MessageTypes.h"
long long getCurrentTimeInMillis() {
	// Get the current time point
	auto now = std::chrono::system_clock::now();

	// Convert time point to milliseconds since epoch
	return static_cast<long long>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count());
}


//This is the Parent Class for ALL messages in the project
class Message {
public:
	/*
	* Constructor
	*/
	Message() {
	}
	virtual void dummyFunction() {};
	/*
	* --------------Print MSG to Screen----------------
	*/

	/*
	* ----------------------------------Message transportation packaging----------------------------------------------
	*/
	//Serialize the message
	std::string serialize() const {
	}


	// Deserialization method
	static Message deserialize(const char* serializedData) {
	}
	/*
	* ---------------------------------------------------------------------------------------------------------------
	*/
};
#endif