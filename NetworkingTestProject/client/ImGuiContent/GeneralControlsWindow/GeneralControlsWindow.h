#pragma once


void renderGeneralControlWindow(ImVec4 &CursorColor, ImVec4& GridColor, ClientEventListener& eventListener,int& dynamicLightingFlag) {
    //Begin the General Controls window
    ImGui::Begin("Test", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Dynamic Lighting", reinterpret_cast<bool*>(&dynamicLightingFlag));

    ImGui::SliderFloat("lagSimulationTime (ms)", &lagSimulationTime, 0.0f, 100.0f);
    ImGui::SliderFloat("Zoom", &projHeight, 15.0f, 1000.0f);
    ImGui::SliderFloat("Zoom", &projWidth, 15.0f, 1000.0f);
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %f", eventListener.getCameraXTranslation());
    ImGui::Text("Y: %f", eventListener.getCameraYTranslation());
    ImGui::Text("Size of Tile list: %d", GlobalConfigurations::getInstance().getMapTileInformation().size());
    ImGui::Text("Grid Color:");
    ImGui::SameLine();
    ImGui::ColorEdit4("GridColor##3", (float*)&GridColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    GlobalConfigurations::getInstance().setGridColor(glm::vec4(GridColor.x, GridColor.y, GridColor.z, GridColor.w));

    ImGui::Text("Cursor Color:");
    ImGui::SameLine();
    ImGui::ColorEdit4("CursorColor##3", (float*)&CursorColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    GlobalConfigurations::getInstance().setCursorColor(glm::vec4(CursorColor.x, CursorColor.y, CursorColor.z, CursorColor.w));

    static float soundVolume = SoundManager::getInstance().getCurrentSoundGain();
    ImGui::SliderFloat("Sound Volume", &soundVolume, 0.0f, 1.0f);
    SoundManager::getInstance().setSoundGain(soundVolume);


    ImGui::Text("Currently Hovered tile: X: %.1f Y: %.1f", GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.x, GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.y);
    ImGui::Text("Current Player tile: X: %.1f Y: %.1f", GlobalConfigurations::getInstance().getCurrentlPlayerTile().layoutIndex.x, GlobalConfigurations::getInstance().getCurrentlPlayerTile().layoutIndex.y);
    static MeasurmentSystem currentMeasurementSystem = MeasurmentSystem::GRID;
    // Render the selectable item for Type A
    if (ImGui::Selectable("GRID", currentMeasurementSystem == MeasurmentSystem::GRID)) {
        currentMeasurementSystem = MeasurmentSystem::GRID;
    }

    // Render the selectable item for Type B
    if (ImGui::Selectable("GEOMETRIC", currentMeasurementSystem == MeasurmentSystem::GOEMETRIC)) {
        currentMeasurementSystem = MeasurmentSystem::GOEMETRIC;
    }
    GlobalConfigurations::getInstance().setCurrentMeasurementSystem(currentMeasurementSystem);


    static bool  gridPaintMode = false; // Boolean variable to store the toggle state

    // Inside your ImGui window or frame loop
    ImGui::Checkbox("Enable Tile editing", &gridPaintMode);

    // Use the toggleValue variable to control your application logic
    if (gridPaintMode) {
        GlobalConfigurations::getInstance().setgridPaintMode(true);
    }
    else {
        GlobalConfigurations::getInstance().setgridPaintMode(false);
    }

    /*
    for (const auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
        const std::string& playerKey = playerEntry.first;
        const Player& player = playerEntry.second;

        // Create a selectable widget for each player
        if (ImGui::Selectable(playerKey.c_str(), playerName == playerKey)) {
            // Set the playerName variable to the selected player's name
            playerName = playerKey;
        }
    }
    */

    if (ImGui::Button("Cancel Current Concentration Spell")) {
        // Set the callback function here
        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).cancelConcentration();
        MessageFactory factory;
        std::stringstream ss;
        ss << true << '|';
        GlobalConfigurations::getInstance().sendMsg(factory.generateCancelConcentrationMessage(MessageType::CANCELCONCENTRATIONSPELL, 1, getCurrentTimeInSeconds(),
            GlobalConfigurations::getInstance().getPlayerName()
        ).serialize().c_str());
    }

    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f); // Set alpha value to make widgets semi-transparent
    }
    if (ImGui::Button("End Turn")) {
        // Set the callback function here
        PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).setIsTurnComplete(true);

        MessageFactory factory;
        std::stringstream ss;
        ss << true << '|';
        GlobalConfigurations::getInstance().sendMsg(factory.generateUpdatePlayerDataMessage(MessageType::UPDATEPLAYERDATA, 1, getCurrentTimeInSeconds(),
            PlayerFields::ISTURNCOMPLETE, GlobalConfigurations::getInstance().getPlayerName(), std::move(ss)
        ).serialize().c_str());
    }

    if (!PlayerDatabase::getInstance().getPlayer(GlobalConfigurations::getInstance().getPlayerName()).getIsTurnReady()) {
        ImGui::PopStyleVar(); // Restore the original alpha value
    }





    ImGui::End();
}