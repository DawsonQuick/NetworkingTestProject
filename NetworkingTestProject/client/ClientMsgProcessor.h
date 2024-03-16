//Message.h
#pragma once
#ifndef CLIENTMESSAGEPROCESSOR_H
#define CLIENTMESSAGEPROCESSOR_H
#include <string>
#include <iostream>
#include <sstream>
#include "./../Common/Messages/Utils/MessageFactory.h"
#include "./../Common/Objects/Spells/SpellFactory.h"
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
			std::stringstream ss(msg.getPlayer());
			std::vector<std::string> resultspells;
			std::string resultname;
			Position resultposition;
			int resulthealth;
			int resultAC;
			double resultmovementSpeed;
			std::string resultselectedSpell;
			bool resultspellChangedState;
			float resultcurrentSpellShotCount;

			bool resultisCurrentlyConcentrating;
			std::string resultconcentrationSpell;
			int resultconcentrationRoundsLeft;
			float resultconcentrationSpellLocationX;
			float resultconcentrationSpellLocationY;

			bool resultisTurnReady;
			bool resultisTurnComplete;

			char delimiter = '|';
			std::string StartSpell;
			std::getline(ss, resultname, delimiter);

			ss >> resultposition.X >> delimiter >> resultposition.Y >> delimiter >>
				resultposition.Z >> delimiter >> resulthealth >> delimiter >> resultAC >>
				delimiter >> resultmovementSpeed >> delimiter;
			std::getline(ss, StartSpell, delimiter);
			int i = 0;
			while (i < 100) {
				std::getline(ss, StartSpell, delimiter);
				if (StartSpell.compare("EndSpells") != 0) {
					resultspells.push_back(StartSpell);
				}
				else {
					break;
				}
				i++;
			}
			std::getline(ss, resultselectedSpell, delimiter);
			ss >> resultspellChangedState >> delimiter >> resultcurrentSpellShotCount >>
				delimiter >> resultisCurrentlyConcentrating >> delimiter;
			std::getline(ss, resultconcentrationSpell, delimiter);
			ss >> resultconcentrationRoundsLeft >> delimiter
				>> resultconcentrationSpellLocationX >> delimiter >> resultconcentrationSpellLocationY
				>> delimiter >> resultisTurnReady >>
				delimiter >> resultisTurnComplete;


			Player newPlayer;
			newPlayer.setName(resultname);
			newPlayer.setPosition(resultposition.X, resultposition.Y, resultposition.Z, 0.0);
			newPlayer.setHealth(resulthealth);
			newPlayer.setAC(resultAC);
			newPlayer.setMovementSpeed(resultmovementSpeed);
			newPlayer.setRoundsRemainingForConcentrationSpell(resultconcentrationRoundsLeft);
			newPlayer.setIsTurnReady(resultisTurnReady);
			newPlayer.setIsTurnComplete(resultisTurnComplete);
			newPlayer.setCurrentSpellShotCount(resultcurrentSpellShotCount);
			newPlayer.setIsCurrentlyConcentrating(resultisCurrentlyConcentrating);
			newPlayer.setConcentrationSpellLocationX(resultconcentrationSpellLocationX);
			newPlayer.setConcentrationSpellLocationX(resultconcentrationSpellLocationY);
			for (std::string spell : resultspells) {
				newPlayer.addSpell(
					SpellFactory::getInstance().stringNametoPointer(spell));
			}
			if (resultselectedSpell != "None") { newPlayer.setSelectedSpell(SpellFactory::getInstance().stringNametoPointer(resultselectedSpell)); }
			if (resultconcentrationSpell != "None") { newPlayer.setConcentrationSpell(SpellFactory::getInstance().stringNametoPointer(resultconcentrationSpell));}

			if (newPlayer.isPlayerCurrentlyConcentrating()) {
				DurationSpellManager::getInstance().addSpell(
					newPlayer.getName(), newPlayer.getCurrentConcentrationSpell()->getName(),
					newPlayer.getCurrentConcentrationSpell()->getSpellType(), newPlayer.getRoundsRemainingForConcentrationSpell() * 6000,
					1000.0f, newPlayer.getCurrentConcentrationSpell()->getImpactRadius(),
					glm::vec2(newPlayer.getConcentrationSpellLocationX(), newPlayer.getConcentrationSpellLocationY()),
						newPlayer.getCurrentConcentrationSpell()->getCallback()
				);
			}
			PlayerDatabase::getInstance().addPlayer(newPlayer.getName(), newPlayer);
			

		}
		else if (ClientDisconnectMessage* testMsg = dynamic_cast<ClientDisconnectMessage*>(msg)) {
			ClientDisconnectMessage& msg = *testMsg;
			msg.toString();
			PlayerDatabase::getInstance().getPlayer(msg.getPlayerName()).cancelConcentration();
			PlayerDatabase::getInstance().removePlayer(msg.getPlayerName());

		}
		else if (CastSpellMessage* testMsg = dynamic_cast<CastSpellMessage*>(msg)) {

			CastSpellMessage& msg = *testMsg;
			std::stringstream ss(msg.getStringStream());
			for (int i = 0; i < msg.getNumberOfShots(); i++) {
				std::string spellName;
				std::string spellType;
				float posX;
				float posY;
				char deliminter = '|';
				std::getline(ss, spellName, deliminter);
				std::getline(ss, spellType, deliminter);
				ss >> posX >> deliminter >> posY >> deliminter;

				Player& player = PlayerDatabase::getInstance().getPlayer(msg.getName());
				player.castSpell(posX,posY);
			}


		}
		else if (CancelConcentrationMessage* testMsg = dynamic_cast<CancelConcentrationMessage*>(msg)) {
			CancelConcentrationMessage& msg = *testMsg;
			Player& player = PlayerDatabase::getInstance().getPlayer(msg.getName());
			player.cancelConcentration();

	}


		else if (UpdatePlayerDataMessage* testMsg = dynamic_cast<UpdatePlayerDataMessage*>(msg)) {
			UpdatePlayerDataMessage& msg = *testMsg;
			Player& player = PlayerDatabase::getInstance().getPlayer(msg.getName());

			switch (msg.getField()) {
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
			case PlayerFields::HEALTH: {
				std::stringstream ss = msg.getStringStream();
				int tmpHealth;
				ss >> tmpHealth;
				player.setHealth(tmpHealth);
				break;
			}
			case PlayerFields::AC: {
				std::stringstream ss = msg.getStringStream();
				int tmpAC;
				ss >> tmpAC;
				player.setAC(tmpAC);
				break;
			}
			case PlayerFields::MOVEMENTSPEED: {
				std::stringstream ss = msg.getStringStream();
				float tmpMovementSpeed;
				char deliminter;
				ss >> tmpMovementSpeed >> deliminter;
				player.setMovementSpeed(tmpMovementSpeed);
				break;
			}
			case PlayerFields::SELECTEDSPELL: {
				std::stringstream ss = msg.getStringStream();
				std::string spellName;
				char deliminter = '|';
				std::getline(ss, spellName, deliminter);

				if (spellName != "None") { player.setSelectedSpell(SpellFactory::getInstance().stringNametoPointer(spellName)); }

				break;
			}
			case PlayerFields::SPELLCHANGESTATE: {
				std::stringstream ss = msg.getStringStream();
				bool spellChangeState;
				char deliminter;

				ss >> spellChangeState >> deliminter;
				//Currently no setter for this member due to only being used locally per client
				break;
			}
			case PlayerFields::CURRENTSPELLSHOTCOUNT: {
				std::stringstream ss = msg.getStringStream();
				float tmpSpellShotCount;
				char deliminter;

				ss >> tmpSpellShotCount >> deliminter;
				player.setCurrentSpellShotCount(tmpSpellShotCount);
				break;
			}
			case PlayerFields::ISCURRENTLYCONCENTRATING: {
				std::stringstream ss = msg.getStringStream();
				bool tmpIsCurrentlyConcentrating;
				char deliminter;

				ss >> tmpIsCurrentlyConcentrating >> deliminter;
				player.setIsCurrentlyConcentrating(tmpIsCurrentlyConcentrating);
				break;
			}
			case PlayerFields::CONCENTRATIONSPELL: {

				std::stringstream ss = msg.getStringStream();
				std::string spellName;
				char deliminter = '|';
				std::getline(ss, spellName, deliminter);

				if (spellName != "None") { player.setConcentrationSpell(SpellFactory::getInstance().stringNametoPointer(spellName)); }

				break;
			}
			case PlayerFields::CONCENTRATIONSPELLLOCATION: {
				std::stringstream ss = msg.getStringStream();
				float tmpConcentrationSpellLocationX;
				float tmpConcentrationSpellLocationY;
				char deliminter;
				ss >> tmpConcentrationSpellLocationX >> deliminter >> tmpConcentrationSpellLocationY >> deliminter;
				player.setConcentrationSpellLocationX(tmpConcentrationSpellLocationX);
				player.setConcentrationSpellLocationY(tmpConcentrationSpellLocationY);
				break;
			}
			case PlayerFields::CONCENTRATIONROUNDSLEFT: {
				std::stringstream ss = msg.getStringStream();
				float tmpConcentrationRoundsLeft;
				char deliminter;
				ss >> tmpConcentrationRoundsLeft >> deliminter;
				player.setRoundsRemainingForConcentrationSpell(tmpConcentrationRoundsLeft);
				break;
			}
			case PlayerFields::ISTURNREADY:{
				std::stringstream ss = msg.getStringStream();
				bool isTurnReady;
				char deliminter;
				ss >> isTurnReady >> deliminter;
				player.setIsTurnReady(isTurnReady);
				break;
			}
			case PlayerFields::ISTURNCOMPLETE: {
				std::stringstream ss = msg.getStringStream();
				bool isTurnComplete;
				char deliminter;

				ss >> isTurnComplete >> deliminter;
				player.setIsTurnComplete(isTurnComplete);
				break;
			}
			
			}
		}
	}


};
#endif