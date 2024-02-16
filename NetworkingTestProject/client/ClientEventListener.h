#pragma once
#ifndef CLIENTEVENTLISTENER_H
#define CLIENTEVENTLISTENER_H
#include "./Client.h"


/*
* -----------------------------------------------------------------------------------------
*                                      Zoom Logic
* -----------------------------------------------------------------------------------------
*/
//TODO change this to be inside the class so these variables are not accessible from the entire program
float projHeight = 100.0;
float projWidth = 100.0;
int scrollSpeed = 10;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!((projWidth - scrollSpeed) <= 0) || !((projHeight - scrollSpeed) <= 0)) {
        if (yoffset > 0) {
            projWidth -= scrollSpeed;
            projHeight -= scrollSpeed;
        }
        else if (yoffset < 0) {
            projWidth += scrollSpeed;
            projHeight += scrollSpeed;
        }
    }
    else {
        projWidth = scrollSpeed + 1; //Set to plus 1 so doesnt get trapped in if condition
        projHeight = scrollSpeed + 1; //Set to plus 1 so doesnt get trapped in if condition
        std::cout << "Cannot scroll in further!" << std::endl;
    }

    std::cout << "Width : " << projWidth << " Height: " << projHeight << std::endl;
}
/*
*          The logic above changes the projection mat 4 used with the shader
* -----------------------------------------------------------------------------------------
*/
class ClientEventListener {

private:
    ClientConnection client;
public:
	//Constructor
	ClientEventListener(ClientConnection &client , GLFWwindow* window ): client(client){
        // Set the scroll wheel callback
        glfwSetScrollCallback(window, scroll_callback);
	}

    /*
* -----------------------------------------------------------------------------------------
*                                    Control Logic
* -----------------------------------------------------------------------------------------
*/
    void processInput(GLFWwindow* windowLocal)
    {


        float prevX = PlayerDatabase::getInstance().getPlayer(playerName).getPositionX();
        float prevY = PlayerDatabase::getInstance().getPlayer(playerName).getPositionY();
        float prevZ = PlayerDatabase::getInstance().getPlayer(playerName).getPositionZ();
        float playerSpeed = 0.1;
        if (glfwGetKey(windowLocal, GLFW_KEY_W) == GLFW_PRESS) {
            PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(prevY + playerSpeed);


            UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::POSITION, playerName, StringStreamer::createStream({ prevX,prevY + playerSpeed,prevZ }));
            std::string msgTest = msg.serialize();
            const char* msgToSend = msgTest.c_str();
            client.sendMsg(msgToSend);

        }
        if (glfwGetKey(windowLocal, GLFW_KEY_S) == GLFW_PRESS) {
            PlayerDatabase::getInstance().getPlayer(playerName).setPositionY(prevY - playerSpeed);

            UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::POSITION, playerName, StringStreamer::createStream({ prevX,prevY - playerSpeed,prevZ }));
            std::string msgTest = msg.serialize();
            const char* msgToSend = msgTest.c_str();
            client.sendMsg(msgToSend);

        }
        if (glfwGetKey(windowLocal, GLFW_KEY_A) == GLFW_PRESS) {
            PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(prevX - playerSpeed);

            UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::POSITION, playerName, StringStreamer::createStream({ prevX - playerSpeed,prevY,prevZ }));
            std::string msgTest = msg.serialize();
            const char* msgToSend = msgTest.c_str();
            client.sendMsg(msgToSend);

        }
        if (glfwGetKey(windowLocal, GLFW_KEY_D) == GLFW_PRESS) {
            PlayerDatabase::getInstance().getPlayer(playerName).setPositionX(prevX + playerSpeed);

            UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::POSITION, playerName, StringStreamer::createStream({ prevX + playerSpeed,prevY,prevZ }));
            std::string msgTest = msg.serialize();
            const char* msgToSend = msgTest.c_str();
            client.sendMsg(msgToSend);


        }


    }
    /*
    *        The logic above detects key presses and moved the player accordingly
    * -----------------------------------------------------------------------------------------
    */
};
#endif