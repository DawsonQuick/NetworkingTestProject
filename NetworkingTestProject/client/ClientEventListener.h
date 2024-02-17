#pragma once
#ifndef CLIENTEVENTLISTENER_H
#define CLIENTEVENTLISTENER_H
#include "./Client.h"
#include "./../OpenGL/vendor/glm/glm.hpp"
#include "./../OpenGL/vendor/glm/gtc/matrix_transform.hpp"

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

// Define global variables to track mouse movement
double lastX = 0.0;
double lastY = 0.0;
bool isDragging = false;
// Define view matrix
glm::mat4 view = glm::mat4(1.0f);
const float panningSpeed = 0.1f;
float totalDistanceTraveledPerHold = 0.0;
// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE) {
            isDragging = false;
            if (totalDistanceTraveledPerHold < 15) {
                std::cout << "Click Detected" << std::endl;
                //TODO: Add events for clicks
            }
            totalDistanceTraveledPerHold = 0.0;
        }
    }
}

// Mouse movement callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {

        double deltaX = (xpos - lastX);
        double deltaY = (ypos - lastY);
        double distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        totalDistanceTraveledPerHold += distance;

        // Calculate mouse movement
        deltaX = (xpos - lastX) * panningSpeed;
        deltaY = (ypos - lastY) * panningSpeed;

        // Update last position
        lastX = xpos;
        lastY = ypos;

        // Perform panning
        view = glm::translate(view, glm::vec3(deltaX, -deltaY, 0.0f));
        


    }
}

class ClientEventListener {

private:
    ClientConnection client;
public:
	//Constructor
	ClientEventListener(ClientConnection &client , GLFWwindow* window ): client(client){
        // Set the scroll wheel callback
        glfwSetScrollCallback(window, scroll_callback);
        // Set mouse button callback
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        // Set cursor position callback
        glfwSetCursorPosCallback(window, cursor_position_callback);
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