//Message.h
#ifndef CLIENTMESSAGEPROCESSOR_H
#define CLIENTMESSAGEPROCESSOR_H
#include <string>
#include <iostream>
#include <sstream>
#include "./../Common/Messages/Utils/MessageFactory.h"
#include "./../Common/Utils/PlayerDatabase.h"


float lagSimulationTime;
class ClientMessageProcessor {

private:
	MessageFactory msgFactory;
public:
	/*
	* Constructor
	*/
	ClientMessageProcessor() {

	}

	void processMessage(const char* inMsg) {
		Message* msg = msgFactory.createMessage(inMsg);
		Sleep(lagSimulationTime);
		if (HandShakeMessage* testMsg = dynamic_cast<HandShakeMessage*>(msg)) {
			std::cout << "Received a TestMessage: " << std::endl;
			HandShakeMessage& msg = *testMsg;
			msg.toString();
		}
		else if (BroadCastMessage* testMsg = dynamic_cast<BroadCastMessage*>(msg)) {
			std::cout << "Received a BroadCastMessage" << std::endl;
			BroadCastMessage& msg = *testMsg;
			msg.toString();
		}
		else if (AddPlayerMessage* testMsg = dynamic_cast<AddPlayerMessage*>(msg)) {
			std::cout << "Received a AddPlayerMessage" << std::endl;
			AddPlayerMessage& msg = *testMsg;
			msg.toString();
			PlayerDatabase::getInstance().addPlayer(msg.getPlayer().getName(),msg.getPlayer());
		}
		else if (ClientDisconnectMessage* testMsg = dynamic_cast<ClientDisconnectMessage*>(msg)) {
			ClientDisconnectMessage& msg = *testMsg;
			msg.toString();
			PlayerDatabase::getInstance().removePlayer(msg.getPlayerName());

		}
		else if (UpdatePlayerDataMessage* testMsg = dynamic_cast<UpdatePlayerDataMessage*>(msg)) {
			//std::cout << "Received a UpdatePlayerDataMessage" << std::endl; //Commented out due to console spam
			UpdatePlayerDataMessage& msg = *testMsg;
			//msg.toString();
			Player& player = PlayerDatabase::getInstance().getPlayer(msg.getName());

			switch (msg.getField()) {
			case PlayerFields::AC: {
				std::stringstream ss = msg.getStringStream();
				int tmpAC;
				ss >> tmpAC;
				player.setAC(tmpAC);
				break;
			}
			case PlayerFields::HEALTH: {
				std::stringstream ss = msg.getStringStream();
				int tmpHealth;
				ss >> tmpHealth;
				player.setHealth(tmpHealth);
				break;
			}
			case PlayerFields::NAME: {
				std::stringstream ss = msg.getStringStream();
				break;
			}
			case PlayerFields::POSITION: {
				std::stringstream ss = msg.getStringStream();
				double posX, posY, posZ;
				char deliminter;
				ss >> posX >> deliminter >> posY >> deliminter >> posZ >> deliminter;
				player.setPosition(posX,posY,posZ,msg.getPointTime());
				break;
			}
			case PlayerFields::KEYPRESS: {
				std::stringstream ss = msg.getStringStream();
				bool W, A, S, D;
				float posX, posY;
				char delimiter;
				ss >> W >> delimiter >> A >> delimiter >> S >> delimiter >> D >> delimiter>>posX>>delimiter>>posY;
				KeyPress keyMap(W, A, S, D);
				player.setKeyPress(keyMap);
				player.setPositionX(posX);
				player.setPositionY(posY);
				break;
			}
			}
		}
	}


};
#endif