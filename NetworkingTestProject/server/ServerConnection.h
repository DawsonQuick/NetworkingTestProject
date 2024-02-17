#pragma on
#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H
#include <iostream>
#include <thread>
#include <list>
#include <atomic>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include "./../Common/Messages/Utils/MessageFactory.h"
#include "./../Common/Utils/PlayerDatabase.h"
#include "./../Common/Utils/Logger.h"
//GLOBAL VARIABLES HERE
std::list<int> clientConnections;
std::atomic<bool> serverRunning(false);
MessageFactory msgFactory;
void processMessage(int clientSocket, const char* inMsg);

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

			for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
				std::string name = playerEntry.first;
					KeyPress keyPress = PlayerDatabase::getInstance().getPlayer(name).getKeyPress();
					Position prevPos = PlayerDatabase::getInstance().getPlayer(name).getPosition();
					double playerSpeed = PlayerDatabase::getInstance().getPlayer(name).getMovementSpeed();
					Position newPos;
					if (keyPress.keyW) {
						PlayerDatabase::getInstance().getPlayer(name).setPositionY((prevPos.Y + playerSpeed));
					}
					if (keyPress.keyA) {
						PlayerDatabase::getInstance().getPlayer(name).setPositionX((prevPos.X - playerSpeed));
					}
					if (keyPress.keyS) {
						PlayerDatabase::getInstance().getPlayer(name).setPositionY((prevPos.Y - playerSpeed));
					}
					if (keyPress.keyD) {
						PlayerDatabase::getInstance().getPlayer(name).setPositionX((prevPos.X + playerSpeed));
					}
				
			
			}

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
		for (auto const& entry : PlayerDatabase::getInstance().getPlayers()) {
			AddPlayerMessage msg(MessageType::ADDPLAYER, 3, 0.0, entry.second);
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
		Logger::getInstance().info("Received a AddPlayerMessage: \n"+msg.toString());


		//If the message is a add player command, then add the player to the server database
		//Then broadcast the command out to all other clients to add to their local database
		PlayerDatabase::getInstance().addPlayer(msg.getPlayer().getName(), msg.getPlayer());
		ServerConnection::noFeedBackBroadcast(clientSocket,inMsg);
	}
	else if (ClientDisconnectMessage* testMsg = dynamic_cast<ClientDisconnectMessage*>(msg)) {
		ClientDisconnectMessage& msg = *testMsg;
		Logger::getInstance().info("Received a ClientDisconnectMessage: \n" + msg.toString());
		PlayerDatabase::getInstance().removePlayer(msg.getPlayerName());

		//Let all the clients know that this client has disconnected
		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);

		//Remove player from list of clients so other clients dont attempt to send to the client and also close the receiver thread of client
		clientConnections.remove(clientSocket);
		//Then close the winsock client connection
		closesocket(clientSocket);
	}
	else if (UpdatePlayerDataMessage* testMsg = dynamic_cast<UpdatePlayerDataMessage*>(msg)) {
		UpdatePlayerDataMessage& msg = *testMsg;

		std::cout << "Received a UpdatePlayerDataMessage" << std::endl; 
		msg.toString();
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
			player.setPosition(posX, posY, posZ,msg.getPointTime());
			break;
		}
		case PlayerFields::KEYPRESS: {
			std::stringstream ss = msg.getStringStream();
			bool W, A, S, D;
			ss >> W >> A >> S >> D;
			KeyPress keyMap(W, A, S, D);
			player.setKeyPress(keyMap);
			break;
		}
		}
		ServerConnection::noFeedBackBroadcast(clientSocket, inMsg);
	}
}