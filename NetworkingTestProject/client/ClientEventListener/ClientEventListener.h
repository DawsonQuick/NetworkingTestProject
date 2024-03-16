#pragma once
#ifndef CLIENTEVENTLISTENER_H
#define CLIENTEVENTLISTENER_H
#include "./../ClientConnection.h"
#include "./../../OpenGL/vendor/glm/glm.hpp"
#include "./../../OpenGL/vendor/glm/gtc/matrix_transform.hpp"
#include "./../../OpenGL/vendor/imgui/imgui.h"
#include "./../../Common/Objects/Spells/MagicMissle.h"
#include "InputProcessor.h"
#include <algorithm>
#include <execution>
#include <atomic>
#include <chrono>
#include <cmath>
std::chrono::milliseconds updateInterval(50); // Update every 100 milliseconds
auto lastUpdateTime = std::chrono::steady_clock::now();
std::atomic<bool> dataReady(false);
std::atomic<std::pair<float, float>> sharedVec({ 0.0f, 0.0f });


//Set the initial dimensions when the GLFW window first intializes
int WIDTH = 1000, HEIGHT = 1000;

//Think if these variables as the "Zoom" for the 2d top-down engine (make sure projWidth and Height always match or warping can occur)
float projHeight = 500.0;
float projWidth = 500.0;

float aspectRatio = static_cast<float>(WIDTH) / HEIGHT;

// Define a window resize callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    // Update OpenGL viewport size
    glViewport(0, 0, width, height);

    std::cout << "Deteting resize: Width: " << width << " Height: " << height << std::endl;
    WIDTH = width;
    HEIGHT = height;
    aspectRatio = static_cast<float>(width) / height;
}



/*
* -----------------------------------------------------------------------------------------
*                                      Zoom Logic
* -----------------------------------------------------------------------------------------
*/
int scrollSpeed = 10;
bool isGUIClicked;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

    //Set a cap to projection of positive values , if projection goes negative things get flipped
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



// Define view matrix
glm::mat4 view = glm::mat4(1.0f);
const float panningSpeed = 0.1f;

// Variables used to detect a mouse click vs a click and drag
float totalDistanceTraveledPerHold = 0.0;
bool isDragging = false;


double mousePosX;
double mousePosY;


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
                float clipX = (2.0f * tempX) / WIDTH - 1.0f;
                float clipY = 1.0f - (2.0f * tempY) / HEIGHT;

                glm::vec4 clipPos(clipX, clipY, 0.0f, 1.0f);
                glm::mat4 tmpProjection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight, projHeight, -1.0f, 1.0f);
                // Apply inverse projection matrix to convert from clip space to NDC
                glm::mat4 inverseProjection = glm::inverse(tmpProjection);
                glm::vec4 ndcPos = inverseProjection * clipPos;
                ndcPos /= ndcPos.w; // Divide by w to get normalized device coordinates

                // Apply inverse view matrix to convert from NDC to world space
                glm::mat4 inverseView = glm::inverse(view);
                glm::vec4 worldPos = inverseView * ndcPos;

                std::cout << "Click Detected at: X: "<< worldPos.x <<" Y: "<< worldPos.y << std::endl;

                
                 // Test Spawns a new player at click location

                long long before = getCurrentTimeInMillis();
                /*
                for (int i = 0; i <= 2000; i++) {
                    std::unique_ptr<Particle> particle = std::make_unique<TestPartical>(worldPos.x, worldPos.y);
                    ParticalDatabase::getInstance().addPartical("Test"+std::to_string(i), std::move(particle));
                }
                */
                processClick(mousePosX, mousePosY);

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

        float clipX = (2.0f * xpos) / WIDTH - 1.0f;
        float clipY = 1.0f - (2.0f * ypos) / HEIGHT;

        glm::vec4 clipPos(clipX, clipY, 0.0f, 1.0f);
        glm::mat4 tmpProjection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight, projHeight, -1.0f, 1.0f);
        // Apply inverse projection matrix to convert from clip space to NDC
        glm::mat4 inverseProjection = glm::inverse(tmpProjection);
        glm::vec4 ndcPos = inverseProjection * clipPos;
        ndcPos /= ndcPos.w; // Divide by w to get normalized device coordinates

        // Apply inverse view matrix to convert from NDC to world space
        glm::mat4 inverseView = glm::inverse(view);
        glm::vec4 worldPos = inverseView * ndcPos;
        auto currentTime = std::chrono::steady_clock::now();
        if (currentTime - lastUpdateTime >= updateInterval) {
            std::pair<float, float> newDataPair(worldPos.x+(GlobalConfigurations::getInstance().getScale()/2), worldPos.y +(GlobalConfigurations::getInstance().getScale() / 2));
            sharedVec.store(newDataPair, std::memory_order_relaxed);
            dataReady.store(true, std::memory_order_release); // Flag data as ready
            lastUpdateTime = currentTime; // Update last update time
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

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        std::thread cursorCalculationThread(&ClientEventListener::calculateClosestGridCellThread, this);
        cursorCalculationThread.detach();
	}

/*
* -----------------------------------------------------------------------------------------
*                                    Control Logic
* -----------------------------------------------------------------------------------------
*/
    //TODO Convert this callback to move the camera and not the player now that there is a movment system in game
    void processInput(GLFWwindow* windowLocal)
    {
        float speed = 2.0f;
        if (glfwGetKey(windowLocal, GLFW_KEY_W) == GLFW_PRESS) {
            view = glm::translate(view, glm::vec3(0.0f, -speed, 0.0f));
        }
             
        if (glfwGetKey(windowLocal, GLFW_KEY_A) == GLFW_PRESS) {
            view = glm::translate(view, glm::vec3(speed, 0.0f, 0.0f));
        }
        if (glfwGetKey(windowLocal, GLFW_KEY_S) == GLFW_PRESS) {
            view = glm::translate(view, glm::vec3(0.0f, speed, 0.0f));
        }
        if (glfwGetKey(windowLocal, GLFW_KEY_D) == GLFW_PRESS) {
            view = glm::translate(view, glm::vec3(-speed, 0.0f, 0.0f));
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




    std::vector<float> getCursor() {

            std::vector<float> playerCursor;

            //Calculate a representation of effected area of currently selected action
            calculateCursor(mousePosX, mousePosY, playerCursor);

            //Calculate a representation of the range that the currently selected action has
            calculateRange(playerCursor);



            return playerCursor;
    }


    bool isPointInsideSquare(glm::vec2 point, glm::mat4x3 square) {
        // Check if the point's x-coordinate is within the square's boundaries
        if (point.x >= square[3][0] && point.x <= square[1][0]) {
            // Check if the point's y-coordinate is within the square's boundaries
            if (point.y >= square[1][1] && point.y <= square[3][1]) {
                return true; // Point is inside the square
            }
        }
        return false; // Point is outside the square
    }


    void calculateClosestGridCellThread() {
        while (true) {
            bool ready = dataReady.load(std::memory_order_acquire); // Acquire the flag value
            if (ready) {
                std::pair<float, float> dataPair = sharedVec.load(std::memory_order_relaxed);
                glm::vec2 dataVec(dataPair.first, dataPair.second);
                std::vector<TileInfo> tmpTileInfo = GlobalConfigurations::getInstance().getMapTileInformation();

                glm::vec2 playerPos(PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionX(), PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getPositionY());

                float tmpScale = GlobalConfigurations::getInstance().getScale();
                TileInfo closestTile;
                TileInfo playerTile;
                std::for_each(std::execution::par, tmpTileInfo.begin(), tmpTileInfo.end(), [&](TileInfo tile) {

                    if (isPointInsideSquare(dataVec, tile.vertexPos)) {
                        closestTile = tile;
                    }
                    else if (isPointInsideSquare(playerPos, tile.vertexPos)) {
                        playerTile = tile;
                    }

                    });
                dataReady.store(false, std::memory_order_release); // Reset the flag
                GlobalConfigurations::getInstance().setCurrentlyHoveredTile(closestTile);
                GlobalConfigurations::getInstance().setCurrentlPlayerTile(playerTile);
                mousePosX = closestTile.centerPos.x;
                mousePosY = closestTile.centerPos.y;
            }

            //Add a bit of time between loops , checking for the dataReady flag without delay really skyrockets cpu usage
            Sleep(10);
        }

    }


};
#endif