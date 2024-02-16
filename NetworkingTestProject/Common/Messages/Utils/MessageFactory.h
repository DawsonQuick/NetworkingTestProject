//Message.h
#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H
#include <string>
#include <iostream>
#include <sstream>
#include "./../../Enums/MessageTypes.h"
#include "./../BroadCastMessage.h"
#include "./../HandShakeMessage.h"
#include "./../AddPlayerMessage.h"
#include "./../UpdatePlayerDataMessage.h"
#include "./../ClientDisconnectMessage.h"
class MessageFactory {

private:
	HandShakeMessage* createTestMessage(const char* serializedData) {
		return HandShakeMessage::deserialize(serializedData);
	}
	BroadCastMessage* createBroadCastMessage(const char* serializedData) {
		return BroadCastMessage::deserialize(serializedData);
	}
	AddPlayerMessage* createAddPlayerMessage(const char* serializedData) {
		return AddPlayerMessage::deserialize(serializedData);
	}
	UpdatePlayerDataMessage* createUpdatePlayerDataMessage(const char* serializedData) {
		return UpdatePlayerDataMessage::deserialize(serializedData);
	}
	ClientDisconnectMessage* createClientDisconnectMessage(const char* serializedData) {
		return ClientDisconnectMessage::deserialize(serializedData);
	}
public:
	/*
	* Constructor
	*/
	MessageFactory() {
	}
	Message* createMessage(const char* serializedData) {
		MessageType identifier;
		std::stringstream ss(serializedData);
		int enumID;
		char delimiter;
		ss >> enumID >> delimiter;

		identifier = static_cast<MessageType>(enumID);
		switch (identifier) {
			case MessageType::HANDSHAKE:
				return createTestMessage(serializedData);
			case MessageType::BROADCAST:
				return createBroadCastMessage(serializedData);
			case MessageType::ADDPLAYER:
				return createAddPlayerMessage(serializedData);
			case MessageType::UPDATEPLAYERDATA:
				return createUpdatePlayerDataMessage(serializedData);
			case MessageType::CLIENTDISCONNECT:
				return createClientDisconnectMessage(serializedData);
			default:
				std::cout << "Unrecognized message, could not create message!" << std::endl;
				return NULL;
		}
	}


};
#endif