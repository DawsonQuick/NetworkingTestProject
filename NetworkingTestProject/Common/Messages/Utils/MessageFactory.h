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
#include "./../CastSpellMessage.h"
#include "./../CancelConcentrationMessage.h"
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
	CastSpellMessage* createCastSpellMessage(const char* serializedData) {
		return CastSpellMessage::deserialize(serializedData);
	}
	CancelConcentrationMessage* createCancelConcentrationSpellMessage(const char* serializedData) {
		return CancelConcentrationMessage::deserialize(serializedData);
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
			case MessageType::CASTSPELL:
				return createCastSpellMessage(serializedData);
			case MessageType::CANCELCONCENTRATIONSPELL:
				return createCancelConcentrationSpellMessage(serializedData);
			default:
				std::cout << "Unrecognized message, could not create message!" << std::endl;
				return NULL;
		}
	}


	HandShakeMessage generateHandShakeMessage(MessageType tempType, int tmpUID, double tmpcreationTime) {
		return HandShakeMessage(tempType,tmpUID, tmpcreationTime);
	}

	BroadCastMessage generateBroadCastMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpMsg) {
		return BroadCastMessage(tempType, tmpUID, tmpcreationTime,tmpMsg);
	}

	AddPlayerMessage generateAddPlayerMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::stringstream tmpPlayer) {
		return AddPlayerMessage(tempType, tmpUID, tmpcreationTime, std::move(tmpPlayer));
	}

	UpdatePlayerDataMessage generateUpdatePlayerDataMessage(MessageType tempType, int tmpUID, double tmpcreationTime, PlayerFields field, std::string tmpName, std::stringstream&& tmpSS) {
		return UpdatePlayerDataMessage(tempType,tmpUID,tmpcreationTime, field, tmpName, std::move(tmpSS));
	}
	UpdatePlayerDataMessage generateUpdatePlayerDataMessage(MessageType tempType, int tmpUID, double tmpcreationTime, PlayerFields field, std::string tmpName, long long tmpPointTime, std::stringstream&& tmpSS) {
		return UpdatePlayerDataMessage(tempType, tmpUID, tmpcreationTime, field, tmpName, tmpPointTime, std::move(tmpSS));
	}
	ClientDisconnectMessage generateClientDisconnectMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpName) {
		return ClientDisconnectMessage(tempType, tmpUID, tmpcreationTime,tmpName);
	}
	CastSpellMessage generateCastSpellMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpName,float numOfShots, std::stringstream&& tmpSS) {
		return CastSpellMessage(tempType, tmpUID, tmpcreationTime,tmpName, numOfShots, std::move(tmpSS));
	}
	CancelConcentrationMessage generateCancelConcentrationMessage(MessageType tempType, int tmpUID, double tmpcreationTime, std::string tmpName) {
		return CancelConcentrationMessage(tempType,tmpUID,tmpcreationTime,tmpName);
	}

};
#endif