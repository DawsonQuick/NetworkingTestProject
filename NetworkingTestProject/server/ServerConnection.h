#pragma on
#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <iostream>
#include <thread>
#include <list>
#include <atomic>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <map>
#include "./../Common/Messages/Utils/MessageFactory.h"
#include "./../Common/Utils/PlayerDatabase.h"
#include "./../Common/Utils/Logger.h"
#include "./../Common/Enums/PlayerFields.h"
//GLOBAL VARIABLES HERE
std::list<int> clientConnections;
std::atomic<bool> serverRunning(false);
MessageFactory msgFactory;
void processMessage(int clientSocket, const char* inMsg);

struct PlayerVariables {
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
	float resultconcentrationSpellLocationX;
	float resultconcentrationSpellLocationY;
	int resultconcentrationRoundsLeft;

	bool resultisTurnReady;
	bool resultisTurnComplete;

	std::stringstream serialize() {
		std::stringstream ss;

		ss << resultname << "|" << resultposition.X << "|" << resultposition.Y << "|" << resultposition.Z
			<< "|" << resulthealth << "|" << resultAC << "|" << resultmovementSpeed << "|"
			<< "StartSpells"
			<< "|";
		for (std::string spell : resultspells) {
			ss << spell << "|";
		}
		ss << "EndSpells"
			<< "|" << resultselectedSpell << "|" << resultspellChangedState << "|"
			<< resultcurrentSpellShotCount << "|" << resultisCurrentlyConcentrating << "|"
			<< resultconcentrationSpell << "|" << resultconcentrationRoundsLeft << "|" 
			<< resultconcentrationSpellLocationX << "|"<< resultconcentrationSpellLocationY<<"|"
			<< resultisTurnReady << "|" << resultisTurnComplete;
		return ss;

	}
};

	std::map<std::string, PlayerVariables> playerMap;
class ServerConnection {
private:

public:
	int port;
	int serverSocket;
	//Constructor
	ServerConnection(int port)
		:port(port) 
	{
		// Initialize Winsock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			Logger::getInstance().error("Failed to initialize Winsock");
		}

		if (serverRunning) {
			Logger::getInstance().error("Server is already running");
		}

		serverSocket = socket(AF_INET, SOCK_STREAM, 0);

		if (serverSocket == INVALID_SOCKET) {
			Logger::getInstance().error("Error creating socket: " + std::to_string(WSAGetLastError()));
		}

		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = INADDR_ANY;
		serverAddress.sin_port = htons(port);

		if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
			Logger::getInstance().error("Error binding socket: " + std::to_string(WSAGetLastError()));
			closesocket(serverSocket);
		}

		if (listen(serverSocket, 10) == SOCKET_ERROR) {
			Logger::getInstance().error("Error listening on socket: " + std::to_string(WSAGetLastError()));
			closesocket(serverSocket);
		}
		serverRunning = true;
		Logger::getInstance().info("Server listening on port " + std::to_string(port) + "..." );

	}
	void acceptConnections(int serverSocket) {
		while (serverRunning) {
			// Accept incoming connections
			int clientSocket = accept(serverSocket, nullptr, nullptr);
			//TODO: add logic to validate connection through a handshake with a password or key
			//If the validation fails then remove the connection
			if (clientSocket != INVALID_SOCKET) {
				// Add the connection to the map
				{
					Logger::getInstance().info("Client has connected: " + std::to_string(clientSocket) +"\n");
					clientConnections.push_back(clientSocket);
					//Start of a receiver thread for the particular client connection
					std::thread clientSes(&ServerConnection::clientReceiver, this, clientSocket);
					clientSes.detach();
				}
			}
		}
	}


	//This is a 'template' for the receiver to the client
	//should be called by a thread to run in the background
	void clientReceiver(int clientSocket) {
		while (serverRunning) {
			char buffer[1024];
			auto it = std::find(clientConnections.begin(), clientConnections.end(), clientSocket);
			if (it == clientConnections.end()) {
				Logger::getInstance().info("Client Disconnected dropping receiver thread!");
				break;
			}
			int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
			if (bytesReceived > 0) {
				// Process the received data
				buffer[bytesReceived] = '\0';
				processMessage(clientSocket,buffer);
			}

		}
	}

	//Send a message out to a client
	static void sendMsg(int clientSocket, const char* msg) {
		send(clientSocket, msg, strlen(msg), 0);
	}

	//Send the message out to all current clients
	static void broadCast(const char* msg) {
		for (int clientConnection : clientConnections) {
			sendMsg(clientConnection, msg);
		}
	}

	//Send a message out to all clients except to the sender of the message
	static void noFeedBackBroadcast(int clientSocket,const char* msg) {
		for (int clientConnection : clientConnections) {
			if (clientConnection != clientSocket) {
				sendMsg(clientConnection, msg);
			}
		}
	}



	

	~ServerConnection() {

	}

	void Start() {
		// Start the accept connections thread
		std::thread acceptThread(&ServerConnection::acceptConnections, this, serverSocket);


		//Main loop for the server class
		//Currently does nothing , just keeps from returning out of the function call
		while (serverRunning) {
		}
	}

	void Stop() {
		closesocket(serverSocket);
		serverRunning = false;
	}



};
#endif


void processMessage(int clientSocket, const char* inMsg) {
	Message* msg = msgFactory.createMessage(inMsg);
	
	if (HandShakeMessage* testMsg = dynamic_cast<HandShakeMessage*>(msg)) {
		HandShakeMessage& msg = *testMsg;
		Logger::getInstance().info("Received a HandShakeMessage: \n"+msg.toString());
		//For response to handshake send all players in servers database to client
		//This syncs the client to the current state of the session
		for (auto& entry : playerMap) {
			AddPlayerMessage msg(MessageType::ADDPLAYER, 3, 0.0, entry.second.serialize());
			std::string msgTest = msg.serialize();
			const char* msgToSend = msgTest.c_str();
			ServerConnection::sendMsg(clientSocket,msgToSend);
		}

		//TODO: Add other information in later to set up client with current state of the session (Like current map)

	}
	else if (BroadCastMessage* testMsg = dynamic_cast<BroadCastMessage*>(msg)) {
		BroadCastMessage& msg = *testMsg;
		Logger::getInstance().info("Received a BroadCastMessage: \n"+msg.toString());
	}


	else if (AddPlayerMessage* testMsg = dynamic_cast<AddPlayerMessage*>(msg)) {

		AddPlayerMessage& msg = *testMsg;
		std::stringstream ss(msg.getPlayer());
		PlayerVariables playerVariables;

		char delimiter = '|';
		std::string StartSpell;
		std::getline(ss, playerVariables.resultname, delimiter);

		ss >> playerVariables.resultposition.X >> delimiter >>
			playerVariables.resultposition.Y >> delimiter >>
			playerVariables.resultposition.Z >> delimiter >>
			playerVariables.resulthealth >> delimiter >> playerVariables.resultAC >>
			delimiter >> playerVariables.resultmovementSpeed >> delimiter;
		std::getline(ss, StartSpell, delimiter);
		int i = 0;
		while (i < 100) {
			std::getline(ss, StartSpell, delimiter);
			if (StartSpell.compare("EndSpells") != 0) {
				playerVariables.resultspells.push_back(StartSpell);
			}
			else {
				break;
			}
			i++;
		}
		std::getline(ss, playerVariables.resultselectedSpell, delimiter);
		ss >> playerVariables.resultspellChangedState >> delimiter >>
			playerVariables.resultcurrentSpellShotCount >> delimiter >>
			playerVariables.resultisCurrentlyConcentrating >> delimiter;
		std::getline(ss, playerVariables.resultconcentrationSpell, delimiter);
		ss >> playerVariables.resultconcentrationRoundsLeft >> delimiter >>
			playerVariables.resultconcentrationSpellLocationX >> delimiter>> playerVariables.resultconcentrationSpellLocationY>> delimiter>>
			playerVariables.resultisTurnReady >> delimiter >>
			playerVariables.resultisTurnComplete;

		playerMap[playerVariables.resultname] = playerVariables;
		ServerConnection::noFeedBackBroadcast(clientSocket,inMsg);
	}


	else if (ClientDisconnectMessage* testMsg = dynamic_cast<ClientDisconnectMessage*>(msg)) {
		ClientDisconnectMessage& msg = *testMsg;
		Logger::getInstance().info("Received a ClientDisconnectMessage: \n" + msg.toString());
		//PlayerDatabase::getInstance().removePlayer(msg.getPlayerName());

		//Let all the clients know that this client has disconnected
		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);

		//Remove player from list of clients so other clients dont attempt to send to the client and also close the receiver thread of client
		clientConnections.remove(clientSocket);
		//Then close the winsock client connection
		closesocket(clientSocket);
	}


	else if (CastSpellMessage* testMsg = dynamic_cast<CastSpellMessage*>(msg)) {
		CastSpellMessage& msg = *testMsg;
		std::cout << "Received a CastSpellMessage" << std::endl;
		msg.toString();
		std::stringstream ss(msg.getStringStream());

		std::string spellName;
		std::string spellType;
		float posX;
		float posY;
		char deliminter = '|';
		std::getline(ss, spellName, deliminter);
		std::getline(ss, spellType, deliminter);
		ss >> posX >> deliminter >> posY >> deliminter;

		//TODO: update server data later , if spell is a certain type, say , concentration

		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);

	}
	else if (CancelConcentrationMessage* testMsg = dynamic_cast<CancelConcentrationMessage*>(msg)) {
		CancelConcentrationMessage& msg = *testMsg;

		//TODO, update player struct on the server to reset the concentration spell

		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);

	}


	else if (UpdatePlayerDataMessage* testMsg = dynamic_cast<UpdatePlayerDataMessage*>(msg)) {
		UpdatePlayerDataMessage& msg = *testMsg;

		std::cout << "Received a UpdatePlayerDataMessage" << std::endl; 
		msg.toString();
		auto& player = playerMap.at(msg.getName());
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
			Position tmpPos(posX, posY, posZ, msg.getPointTime());
			player.resultposition = tmpPos;
			break;
		}
		case PlayerFields::HEALTH: {
			std::stringstream ss = msg.getStringStream();
			int tmpHealth;
			ss >> tmpHealth;
			player.resulthealth = tmpHealth;
			break;
		}
		case PlayerFields::AC: {
			std::stringstream ss = msg.getStringStream();
			int tmpAC;
			ss >> tmpAC;
			player.resultAC = tmpAC;
			break;
		}
		case PlayerFields::MOVEMENTSPEED: {
			std::stringstream ss = msg.getStringStream();
			float tmpMovementSpeed;
			char deliminter;
			ss >> tmpMovementSpeed >> deliminter;
			player.resultmovementSpeed = tmpMovementSpeed;
			break;
		}
		case PlayerFields::SELECTEDSPELL: {
			std::stringstream ss = msg.getStringStream();
			std::string spellName;
			char deliminter = '|';
			std::getline(ss, spellName, deliminter);

			player.resultselectedSpell = spellName;

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
			player.resultcurrentSpellShotCount = tmpSpellShotCount;
			break;
		}
		case PlayerFields::ISCURRENTLYCONCENTRATING: {
			std::stringstream ss = msg.getStringStream();
			bool tmpIsCurrentlyConcentrating;
			char deliminter;

			ss >> tmpIsCurrentlyConcentrating >> deliminter;
			player.resultisCurrentlyConcentrating = tmpIsCurrentlyConcentrating;
			break;
		}
		case PlayerFields::CONCENTRATIONSPELL: {

			std::stringstream ss = msg.getStringStream();
			std::string spellName;
			char deliminter = '|';
			std::getline(ss, spellName, deliminter);

			player.resultconcentrationSpell = spellName;

			break;
		}
		case PlayerFields::CONCENTRATIONSPELLLOCATION: {
			std::stringstream ss = msg.getStringStream();
			float tmpConcentrationSpellLocationX;
			float tmpConcentrationSpellLocationY;
			char deliminter;
			ss >> tmpConcentrationSpellLocationX >> deliminter >> tmpConcentrationSpellLocationY >> deliminter;
			player.resultconcentrationSpellLocationX = tmpConcentrationSpellLocationX;
			player.resultconcentrationSpellLocationY = tmpConcentrationSpellLocationY;
			break;
		}
		case PlayerFields::CONCENTRATIONROUNDSLEFT: {
			std::stringstream ss = msg.getStringStream();
			float tmpConcentrationRoundsLeft;
			char deliminter;
			ss >> tmpConcentrationRoundsLeft >> deliminter;
			player.resultconcentrationRoundsLeft = tmpConcentrationRoundsLeft;
			break;
		}
		case PlayerFields::ISTURNREADY: {
			std::stringstream ss = msg.getStringStream();
			bool isTurnReady;
			char deliminter;
			ss >> isTurnReady >> deliminter;
			player.resultisTurnReady = isTurnReady;
			break;
		}
		case PlayerFields::ISTURNCOMPLETE: {
			std::stringstream ss = msg.getStringStream();
			bool isTurnComplete;
			char deliminter;

			ss >> isTurnComplete >> deliminter;
			player.resultisTurnComplete = isTurnComplete;
			break;
		}
		}
		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);
	}
}