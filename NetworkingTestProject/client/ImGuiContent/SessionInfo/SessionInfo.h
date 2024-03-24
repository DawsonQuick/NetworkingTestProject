#pragma once

void renderSessionInfo() {
    //Beginning of the Session information
    ImGui::Begin("Session Information");

    if (ImGui::TreeNode("Players")) {
        for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
            if (ImGui::TreeNode(playerEntry.second.getName().c_str(), "Player %s", playerEntry.second.getName().c_str())) {
                ImGui::Text("Name: %s", playerEntry.first.c_str());
                ImGui::Text("X: %f Y: %f", playerEntry.second.getPositionX(), playerEntry.second.getPositionY());
                ImGui::Text("Health: %d", playerEntry.second.getHealth());
                ImGui::Text("AC: %d", playerEntry.second.getAC());
                ImGui::Text("MovementSpeed: %f", playerEntry.second.getMovementSpeed());
                if (playerEntry.second.getSelectedSpell() != nullptr) { ImGui::Text("SelectedSpell: %s", playerEntry.second.getSelectedSpell()->getName().c_str()); }
                const char* boolStr = playerEntry.second.isPlayerCurrentlyConcentrating() ? "true" : "false";
                ImGui::Text("Is Player Concentrating %s", boolStr);
                if (boolStr == "true") {
                    ImGui::Text("Current Concentration Spell : %s , with %d rounds remaining", playerEntry.second.getCurrentConcentrationSpellName().c_str(),
                        playerEntry.second.getRoundsRemainingForConcentrationSpell());
                }


                boolStr = playerEntry.second.getIsTurnReady() ? "true" : "false";
                ImGui::Text("Is Player Turn Ready %s", boolStr);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}