#pragma once
#ifndef CLIENTEVENTLISTENER_H
#define CLIENTEVENTLISTENER_H
#include "./Client.h"
#include "./../OpenGL/vendor/glm/glm.hpp"
#include "./../OpenGL/vendor/glm/gtc/matrix_transform.hpp"
#include "./../OpenGL/vendor/imgui/imgui.h"
/*
* -----------------------------------------------------------------------------------------
*                                      Zoom Logic
* -----------------------------------------------------------------------------------------
*/
//TODO change this to be inside the class so these variables are not accessible from the entire program
float projHeight = 70.0;
float projWidth = 70.0;
int scrollSpeed = 10;
bool isGUIClicked;
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
    //Disable this callback if interacting with a ImGUI element
    if (ImGui::IsMouseHoveringAnyWindow() || ImGui::IsAnyItemActive()) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE) {
            isDragging = false;
            if (totalDistanceTraveledPerHold < 15) {
                
                double tempX, tempY;
                glfwGetCursorPos(window, &tempX, &tempY);
                float clipX = (2.0f * tempX) / 1000 - 1.0f;
                float clipY = 1.0f - (2.0f * tempY) / 1000;

                glm::vec4 clipPos(clipX, clipY, 0.0f, 1.0f);
                glm::mat4 tmpProjection = glm::ortho(-projWidth, projWidth, -projHeight, projHeight, -1.0f, 1.0f);
                // Apply inverse projection matrix to convert from clip space to NDC
                glm::mat4 inverseProjection = glm::inverse(tmpProjection);
                glm::vec4 ndcPos = inverseProjection * clipPos;
                ndcPos /= ndcPos.w; // Divide by w to get normalized device coordinates

                // Apply inverse view matrix to convert from NDC to world space
                glm::mat4 inverseView = glm::inverse(view);
                glm::vec4 worldPos = inverseView * ndcPos;

                std::cout << "Click Detected at: X: "<< worldPos.x <<" Y: "<< worldPos.y << std::endl;
                Player newPlayer;
                newPlayer.setPosition(worldPos.x,worldPos.y,0.0,0.0L);
                PlayerDatabase::getInstance().addPlayer("NPCTest:"+std::to_string(worldPos.x), newPlayer);
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
float simulatedLagThreshold;
class ClientEventListener {

private:
    long long timeSinceLastMessage = 0.0;
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
        bool isWPressed = glfwGetKey(windowLocal, GLFW_KEY_W) == GLFW_PRESS;
        bool isAPressed = glfwGetKey(windowLocal, GLFW_KEY_A) == GLFW_PRESS;
        bool isSPressed = glfwGetKey(windowLocal, GLFW_KEY_S) == GLFW_PRESS;
        bool isDPressed = glfwGetKey(windowLocal, GLFW_KEY_D) == GLFW_PRESS;

        KeyPress currentPlayerKeyPress(isWPressed, isAPressed, isSPressed, isDPressed);
        KeyPress previousPlayerKeyPress = PlayerDatabase::getInstance().getPlayer(playerName).getKeyPress();

        if (currentPlayerKeyPress != previousPlayerKeyPress) {
            std::cout << "Change in key press" << std::endl;
            PlayerDatabase::getInstance().getPlayer(playerName).setKeyPress(currentPlayerKeyPress);
            Position pos = PlayerDatabase::getInstance().getPlayer(playerName).getPosition();
            UpdatePlayerDataMessage msg(MessageType::UPDATEPLAYERDATA, 3, getCurrentTimeInSeconds(), PlayerFields::KEYPRESS, playerName, StringStreamer::createStream(currentPlayerKeyPress , pos));
            std::string msgTest = msg.serialize();
            const char* msgToSend = msgTest.c_str();
            client.sendMsg(msgToSend);



        }
    }
    /*
    *        The logic above detects key presses and moved the player accordingly
    * -----------------------------------------------------------------------------------------
    */

    float getCameraXTranslation() {
        return -(view[3][0]);
    }
    float getCameraYTranslation() {
        return -(view[3][1]);
    }
};
#endif