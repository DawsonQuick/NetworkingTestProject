#pragma once

void renderPlayerInformationWindow(int display_w, int display_h) {
    ImGui::Begin("Player Information", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f); // Set alpha value to make widgets semi-transparent
    }

    static std::string currentSelected = "Move"; // Set to "Move" by default
    static std::string currentSpellName;
    static std::string currentWeaponName;

    // Begin a column
    ImGui::Columns(3, "PlayerInfoColumns", false); // 2 columns, don't auto-fit columns
    ImGui::Text("List of available basic actions");
    ImGui::SetColumnWidth(0, display_w / 3);
    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::Text("Player's turn is not ready"); // Show a message instead of the "Move" option
        currentSelected = "NONE";
    }
    else {
        if (ImGui::Selectable("Move", currentSelected == "Move")) {
            currentSelected = "Move";
            currentSpellName = ""; // Reset the selected spell
            currentWeaponName = "";
        }
    }
    ImGui::NextColumn();
    ImGui::SetColumnWidth(1, display_w / 3);
    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::Text("Player's turn is not ready"); // Show a message instead of the "Move" option
        currentSelected = "NONE";
    }
    else {
        for (std::shared_ptr<Weapon> weapon : PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getAvailableWeapons()) {
            if (ImGui::Selectable(weapon->getName().c_str(), currentWeaponName == weapon->getName())) {
                currentWeaponName = weapon->getName();
                currentSelected = "Use Weapon";
                currentSpellName = ""; // Reset the selected spell
                PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setSelectedWeapon(weapon);
            }
        }
    }
    ImGui::NextColumn();
    ImGui::SetColumnWidth(2, display_w / 3);
    ImGui::Text("List of available spells");
    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::Text("Player's turn is not ready"); // Show a message instead of the list of spells
    }
    else {
        for (std::shared_ptr<Spell> spell : PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getAvailableSpells()) {
            if (ImGui::Selectable(spell->getName().c_str(), currentSpellName == spell->getName())) {
                currentSpellName = spell->getName();
                currentSelected = "Cast Spell";
                currentWeaponName = "";
                PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setSelectedSpell(spell);
                MessageFactory factory;
                std::stringstream ss;
                ss << spell->getName() << '|';
                GlobalConfigurations::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, getCurrentTimeInSeconds(),
                    PlayerFields::SELECTEDSPELL, GlobalConfigurations::getInstance().getPlayerName(), std::move(ss)
                ).serialize().c_str());
            }
        }
    }
    ImGui::Columns(1);
    GlobalConfigurations::getInstance().setselectedAction(currentSelected);

    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::PopStyleVar(); // Restore the original alpha value
    }
    // End the window
    ImGui::End();
}