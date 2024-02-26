#pragma once

/*
*  Ties to the main render loop, renders all ImGui related content
*/
void imGuiRender(int &dynamicLightingFlag, ClientEventListener &eventListener) {
    ImGui::Begin("Test");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Dynamic Lighting", reinterpret_cast<bool*>(&dynamicLightingFlag));
    for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
        ImGui::Text("Players: %s", playerEntry.first.c_str());
        ImGui::Text("X: %f Y: %f", playerEntry.second.getPositionX(), playerEntry.second.getPositionY());

        /*
        ImGui::Text("KeyBoard Press: ");
        ImGui::Text("W:"); ImGui::SameLine();
        if (playerEntry.second.getKeyPress().keyW) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor(); }
        else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
        ImGui::Text("A:"); ImGui::SameLine();
        if (playerEntry.second.getKeyPress().keyA) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor(); }
        else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
        ImGui::Text("S:"); ImGui::SameLine();
        if (playerEntry.second.getKeyPress().keyS) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor(); }
        else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
        ImGui::Text("D:"); ImGui::SameLine();
        if (playerEntry.second.getKeyPress().keyD) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor(); }
        else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
        */

    }
    ImGui::SliderFloat("lagSimulationTime (ms)", &lagSimulationTime, 0.0f, 100.0f);
    ImGui::SliderFloat("Zoom", &projHeight, 15.0f, 100.0f);
    ImGui::SliderFloat("Zoom", &projWidth, 15.0f, 100.0f);
    ImGui::Text("Camera Position:");
    ImGui::Text("X: %f", eventListener.getCameraXTranslation());
    ImGui::Text("Y: %f", eventListener.getCameraYTranslation());
    ImGui::End();
    ImGui::Render();
}