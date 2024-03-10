//Message.h
#ifndef CLIENTMESSAGEPROCESSOR_H
#define CLIENTMESSAGEPROCESSOR_H
#include <string>
#include <iostream>
#include <sstream>
#include "./../Common/Messages/Utils/MessageFactory.h"
#include "./../Common/Utils/PlayerDatabase.h"


float lagSimulationTime;
/*
* Classes is used to generate and process messages coming in from the TCP connection to the Client
*/
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
		//Pass the incoming byte stream to the msgFactory to reconstruct message
		Message* msg = msgFactory.createMessage(inMsg);
		Sleep(lagSimulationTime);

		//Next step is to see what type of message was send and preform the appropriate actions

		if (HandShakeMessage* testMsg = dynamic_cast<HandShakeMessage*>(msg)) {
			//std::cout << "Received a TestMessage: " << std::endl;
			HandShakeMessage& msg = *testMsg;
			msg.toString();
		}
		else if (BroadCastMessage* testMsg = dynamic_cast<BroadCastMessage*>(msg)) {
			//std::cout << "Received a BroadCastMessage" << std::endl;
			BroadCastMessage& msg = *testMsg;
			msg.toString();
		}
		else if (AddPlayerMessage* testMsg = dynamic_cast<AddPlayerMessage*>(msg)) {
			//std::cout << "Received a AddPlayerMessage" << std::endl;
			AddPlayerMessage& msg = *testMsg;
			PlayerDatabase::getInstance().addPlayer(msg.getPlayer().getName(),msg.getPlayer());
		}
		else if (ClientDisconnectMessage* testMsg = dynamic_cast<ClientDisconnectMessage*>(msg)) {
			ClientDisconnectMessage& msg = *testMsg;
			msg.toString();
			PlayerDatabase::getInstance().removePlayer(msg.getPlayerName());

		}
		else if (UpdatePlayerDataMessage* testMsg = dynamic_cast<UpdatePlayerDataMessage*>(msg)) {
			UpdatePlayerDataMessage& msg = *testMsg;
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
			}
		}
	}


};
#endif