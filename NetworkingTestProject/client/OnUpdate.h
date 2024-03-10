#pragma once
#include <queue>
#include "./../Common/Objects/Spells/DurationSpellManager.h"
std::queue<float> updateRequest;
//Updates all players positions based on if the player has a key pressed, distance moved is determined by that players movement speed
void UpdatePlayerPosition(std::string name,float deltaTime) {

}




void OnUpdate(float deltaTime) {
    updateRequest.push(deltaTime);

}

void OnUpdateThread() {
    while (true) {
        if (!updateRequest.empty()) {
          //Update all player positions
          for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) { UpdatePlayerPosition(playerEntry.first, updateRequest.front()); }
          DurationSpellManager::getInstance().onUpdate(updateRequest.front());
          ParticleDatabase::getInstance().onUpdate(updateRequest.front());
          updateRequest.pop();
        }
    }
}