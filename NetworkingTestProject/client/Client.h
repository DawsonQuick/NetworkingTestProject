#pragma once
#include <iostream>
#include <thread>
#include <list>
#include <atomic>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include "./ClientMsgProcessor.h"
#include "./../Common/Utils/PlayerDatabase.h"
#include "./../Common/Utils/StringStreamer.h"
//GLOBAL VARIABLES HERE
SOCKET clientSocket;
std::string playerName;

double getCurrentTimeInSeconds() {
    // Get the current time point
    auto currentTime = std::chrono::system_clock::now();

    // Convert the time point to duration since the epoch
    auto durationSinceEpoch = currentTime.time_since_epoch();

    // Convert the duration to seconds as a floating-point value (double)
    double seconds = std::chrono::duration<double>(durationSinceEpoch).count();

    return seconds;
}

class ClientConnection {
private:
    ClientMessageProcessor msgProcessor;
public:

    /*
    * Constructor
    */
	ClientConnection(const char* ServerAddress , int port) {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			std::cerr << "Failed to initialize Winsock" << std::endl;
			// Handle error
		}
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            Sleep(3000);
        }

        // Server address and port
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(ServerAddress); // Server IP address
        serverAddress.sin_port = htons(port); // Server port

        // Connect to the server
        if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
        }

        std::cout << "Connected to the server." << std::endl;

        // Send data to the server
        const char* message = "Hello, server!";
        HandShakeMessage msg(MessageType::HANDSHAKE ,1, getCurrentTimeInSeconds());



        std::string msgTest = msg.serialize();

        const char* msgToSend = msgTest.c_str();

        sendMsg(msgToSend);

	}
    /*
    * Deconstructor
    */
	~ClientConnection() {

	}

    /*
    * Start the main loop of the client
    */
	void Start() {
        std::thread clientSes(&ClientConnection::clientReceiver, this);
        clientSes.detach();

       

       
        
        HandShakeMessage msg(MessageType::HANDSHAKE,1, getCurrentTimeInSeconds());
        std::string msgTest = msg.serialize();
        const char* msgToSend = msgTest.c_str();

        std::cout << "Please enter your charaters name: ";
        std::cin >> playerName;
        Player player(playerName, 0.0, 0.0, 0.0, 55, 15);
        PlayerDatabase::getInstance().addPlayer(playerName, player);
        AddPlayerMessage msg2(MessageType::ADDPLAYER, 3, getCurrentTimeInSeconds(), player);
        msgTest = msg2.serialize();
        msgToSend = msgTest.c_str();
        sendMsg(msgToSend);



        /* 
        std::string input;
        //THIS LOOP IS SIMULATING THE MAIN GAME LOOP
        while (true) {
            std::cout << "Type '1' to send a handshake message" << std::endl;
            std::cout << "Type '2' followed by a space(' ') and then a message to broadcast to other connections" << std::endl;
            std::cout << "Type '3' to send an example message to add a new player to the session" << std::endl;
            std::cout << "Type '4' to show current player list" << std::endl;
            std::cout << "Type '5' to update player field" << std::endl;
            std::cout << "Type '6' to show players info" << std::endl;
            std::cin >> input;
            if (input == "1") {
                sendMsg(msgToSend);
            }
            else if (input == "2") {
                std::string msgIn;
                std::getline(std::cin,msgIn);
                BroadCastMessage msg(MessageType::BROADCAST,1, getCurrentTimeInSeconds(), msgIn);
                std::string msgTest = msg.serialize();
                const char* msgToSend = msgTest.c_str();
                sendMsg(msgToSend);
            }
            else if (input == "3") {
                std::cout << "Please enter your charaters name: ";
                std::cin >> playerName;
                Player player(playerName,55.0,100.0,0.0,55,15);
                PlayerDatabase::getInstance().addPlayer(playerName, player);
                AddPlayerMessage msg(MessageType::ADDPLAYER, 3, getCurrentTimeInSeconds(), player);
                std::string msgTest = msg.serialize();
                const char* msgToSend = msgTest.c_str();
                sendMsg(msgToSend);
            }
            else if (input == "4") {
                PlayerDatabase::getInstance().printCurrentPlayers();
            }
            else if (input == "5") {
                UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::HEALTH, playerName, StringStreamer::createStream(150));
                Player& player = PlayerDatabase::getInstance().getPlayer(playerName);
                player.setHealth(150);
                std::string msgTest = msg.serialize();
                const char* msgToSend = msgTest.c_str();
                sendMsg(msgToSend);
            }
            else if (input == "6") {
                std::string name;
                std::cout << "Enter Players Name: ";
                std::cin >> name;
                Player& player = PlayerDatabase::getInstance().getPlayer(name);
                player.toString();

            }

        }*/
	}
	void Stop() {
        closesocket(clientSocket);
	}

    //The receiver loop for the client session, should be started from a seperate thread
    void clientReceiver() {
        while (true) {
            char buffer[1024];
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                msgProcessor.processMessage(buffer);
            }
        }
    }

    void sendMsg(const char* msg) {
        send(clientSocket, msg, strlen(msg), 0);
    }
    std::string getPlayerName() {
        return playerName;
    }

};

