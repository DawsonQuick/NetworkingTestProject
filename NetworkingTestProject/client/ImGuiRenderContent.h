#pragma once

/*
*  Ties to the main render loop, renders all ImGui related content
*/
void imGuiRender(GLFWwindow* window ,int &dynamicLightingFlag, ClientEventListener &eventListener) {
    // Get screen dimensions
    static ImVec4 GridColor = ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 100.0f / 255.0f);
    static ImVec4 CursorColor = ImVec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
    
    // Get the size of the viewport (the entire window)
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Calculate window position (top-right corner)
    ImVec2 windowPos(display_w - 300, 0);

    //General Controls window
        //Setting the positions of the GeneralControlsWindow
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);

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
    ImGui::ColorEdit4("GridColor##3", (float*)&GridColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
    GlobalConfigurations::getInstance().setGridColor(glm::vec4(GridColor.x, GridColor.y, GridColor.z, GridColor.w));

    ImGui::Text("Cursor Color:");
    ImGui::SameLine();
    ImGui::ColorEdit4("CursorColor##3", (float*)&CursorColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    GlobalConfigurations::getInstance().setCursorColor(glm::vec4(CursorColor.x, CursorColor.y, CursorColor.z, CursorColor.w));

    ImGui::Text("Currently Hovered tile: X: %.1f Y: %.1f", GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.x, GlobalConfigurations::getInstance().getCurrentlyHoveredTile().layoutIndex.y);

    static MeasurmentSystem currentMeasurementSystem;
    // Render the selectable item for Type A
    if (ImGui::Selectable("GRID", currentMeasurementSystem == MeasurmentSystem::GRID)) {
        currentMeasurementSystem = MeasurmentSystem::GRID;
    }

    // Render the selectable item for Type B
    if (ImGui::Selectable("GEOMETRIC", currentMeasurementSystem == MeasurmentSystem::GOEMETRIC)) {
        currentMeasurementSystem = MeasurmentSystem::GOEMETRIC;
    }
    GlobalConfigurations::getInstance().setCurrentMeasurementSystem(currentMeasurementSystem);


    ImGui::End();


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
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();



    ImGui::SetNextWindowPos(ImVec2(0, display_h - 200));
    ImGui::SetNextWindowSize(ImVec2(display_w, 200));


    ImGui::Begin("Player Information", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static std::string currentSelected = "Move"; // Set to "Move" by default
    static std::string currentSpellName;

    // Begin a column
    ImGui::Columns(2, "PlayerInfoColumns", false); // 2 columns, don't auto-fit columns
    ImGui::Text("List of available basic actions");
    ImGui::SetColumnWidth(0, display_w/2);
    if (ImGui::Selectable("Move", currentSelected == "Move")) {
        currentSelected = "Move";
        currentSpellName = ""; // Reset the selected spell
    }
    ImGui::NextColumn(); 

    ImGui::SetColumnWidth(1, display_w/2);
    ImGui::Text("List of available spells");
    for (std::shared_ptr<Spell> spell : PlayerDatabase::getInstance().getPlayer(playerName).getAvailableSpells()) {
        if (ImGui::Selectable(spell->getName().c_str(), currentSpellName == spell->getName())) {
            currentSpellName = spell->getName();
            currentSelected = "Cast Spell";
            GlobalConfigurations::getInstance().setSelectedSpell(spell);
        }
    }
    ImGui::Columns(1);
    GlobalConfigurations::getInstance().setselectedAction(currentSelected);

    // End the window
    ImGui::End();

    ImGui::Render();
}