#pragma once

/*
*  Ties to the main render loop, renders all ImGui related content
*/
void imGuiRender(int &dynamicLightingFlag, ClientEventListener &eventListener) {
    // Get screen dimensions
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int screenWidth = mode->width;
    int screenHeight = mode->height;

    // Calculate window position (top-right corner)
    ImVec2 windowPos(screenWidth - 300, 0);

    // Create a window
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);
    ImGui::Begin("Test", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Dynamic Lighting", reinterpret_cast<bool*>(&dynamicLightingFlag));

    ImGui::SliderFloat("lagSimulationTime (ms)", &lagSimulationTime, 0.0f, 100.0f);
    ImGui::SliderFloat("Zoom", &projHeight, 15.0f, 100.0f);
    ImGui::SliderFloat("Zoom", &projWidth, 15.0f, 100.0f);
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %f", eventListener.getCameraXTranslation());
    ImGui::Text("Y: %f", eventListener.getCameraYTranslation());
    ImGui::Text("Size of Tile list: %d", GlobalConfigurations::getInstance().getMapTileInformation().size());
    ImGui::End();

    ImGui::Begin("Session Information");
    for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
        ImGui::Text("Players: %s", playerEntry.first.c_str());
        ImGui::Text("X: %f Y: %f", playerEntry.second.getPositionX(), playerEntry.second.getPositionY());
        ImGui::Text("Health: %d", playerEntry.second.getHealth());

    }
    ImGui::End();

    ImGui::Render();
}