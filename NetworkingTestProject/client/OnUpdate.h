#pragma once
#include <queue>

std::queue<float> updateRequest;
//Updates all players positions based on if the player has a key pressed, distance moved is determined by that players movement speed
void UpdatePlayerPosition(std::string name,float deltaTime) {
    KeyPress keyPress = PlayerDatabase::getInstance().getPlayer(name).getKeyPress();
    Position prevPos = PlayerDatabase::getInstance().getPlayer(name).getPosition();
    double playerSpeed = PlayerDatabase::getInstance().getPlayer(name).getMovementSpeed();
    Position newPos = prevPos;
    if (keyPress.keyW) {
        newPos.Y = prevPos.Y + (playerSpeed * deltaTime * GlobalConfigurations::getInstance().getScale());
    }
    if (keyPress.keyA) {
        newPos.X = prevPos.X - (playerSpeed * deltaTime * GlobalConfigurations::getInstance().getScale());
    }
    if (keyPress.keyS) {
        newPos.Y = prevPos.Y - (playerSpeed * deltaTime * GlobalConfigurations::getInstance().getScale());
    }
    if (keyPress.keyD) {
        newPos.X = prevPos.X + (playerSpeed * deltaTime * GlobalConfigurations::getInstance().getScale());
    }
    
    //TODO: Add logic for colision detection, 
    //If player hits something, set to previous position
    PlayerDatabase::getInstance().getPlayer(name).setPosition(newPos.X, newPos.Y, newPos.Z, getCurrentTimeInMillis());
}

void checkPlayerHealth(std::string name) {
    if (PlayerDatabase::getInstance().getPlayer(name).getHealth() < 0) {
        PlayerDatabase::getInstance().removePlayer(name);
    }
}



void OnUpdate(float deltaTime) {
    updateRequest.push(deltaTime);

}

void OnUpdateThread() {
    while (true) {
        if (!updateRequest.empty()) {
          //Update all player positions
          for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) { UpdatePlayerPosition(playerEntry.first, updateRequest.front()); }
          for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) { checkPlayerHealth(playerEntry.first); }
          //for (auto& particalEntry : ParticalDatabase::getInstance().getParticals()) { UpdateParticals(particalEntry.first,deltaTime); }
          ParticleDatabase::getInstance().onUpdate(updateRequest.front());
          updateRequest.pop();
        }
    }
}