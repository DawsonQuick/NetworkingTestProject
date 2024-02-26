#pragma once
#include <queue>
float timeToLast = 5000.0; //Time in milliseconds (5 seconds)
float instanceTime = 0.0;
std::queue<float> updateRequest;
//Updates all players positions based on if the player has a key pressed, distance moved is determined by that players movement speed
void UpdatePlayerPosition(std::string name,float deltaTime) {
    KeyPress keyPress = PlayerDatabase::getInstance().getPlayer(name).getKeyPress();
    Position prevPos = PlayerDatabase::getInstance().getPlayer(name).getPosition();
    double playerSpeed = PlayerDatabase::getInstance().getPlayer(name).getMovementSpeed();
    Position newPos = prevPos;
    if (keyPress.keyW) {
        newPos.Y = prevPos.Y + (playerSpeed * deltaTime);
    }
    if (keyPress.keyA) {
        newPos.X = prevPos.X - (playerSpeed * deltaTime);
    }
    if (keyPress.keyS) {
        newPos.Y = prevPos.Y - (playerSpeed * deltaTime);
    }
    if (keyPress.keyD) {
        newPos.X = prevPos.X + (playerSpeed * deltaTime);
    }
    
    //TODO: Add logic for colision detection, 
    //If player hits something, set to previous position
    PlayerDatabase::getInstance().getPlayer(name).setPosition(newPos.X, newPos.Y, newPos.Z, getCurrentTimeInMillis());
}




void OnUpdate(float deltaTime) {
    updateRequest.push(deltaTime);

}

void OnUpdateThread() {
    while (true) {
        if (!updateRequest.empty()) {
          //Update all player positions
          for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) { UpdatePlayerPosition(playerEntry.first, updateRequest.front()); }
          //for (auto& particalEntry : ParticalDatabase::getInstance().getParticals()) { UpdateParticals(particalEntry.first,deltaTime); }
          ParticalDatabase::getInstance().onUpdate(updateRequest.front());
          updateRequest.pop();
        }
    }
}