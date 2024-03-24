#pragma once
#ifndef IMGUIRENDERCONTEXT_H
#define IMGUIRENDERCONTEXT_H
#include "./../../Common/Messages/Utils/MessageFactory.h"
#include "SessionInfo/SessionInfo.h"
#include "GeneralControlsWindow/GeneralControlsWindow.h"
#include "PlayerInformationWindow/PlayerInformationWindow.h"
/*
*  Ties to the main render loop, renders all ImGui related content
*/
void imGuiRender(GLFWwindow* window ,int &dynamicLightingFlag, ClientEventListener &eventListener) {
    // Get screen dimensions
    static ImVec4 GridColor = ImVec4(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 100.0f / 255.0f);
    static ImVec4 CursorColor = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
    
    // Get the size of the viewport (the entire window)
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Calculate window position (top-right corner)
    ImVec2 windowPos(display_w - 300, 0);

    //General Controls window
        //Setting the positions of the GeneralControlsWindow
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Always);

    
    //Displays general information about the client, FPS cursor location, ect
    renderGeneralControlWindow(CursorColor, GridColor , eventListener , dynamicLightingFlag);

    //Displays all information regarding the session, each player and information
    renderSessionInfo();



    ImGui::SetNextWindowPos(ImVec2(0, display_h - 200));
    ImGui::SetNextWindowSize(ImVec2(display_w, 200));


    //Displays the actions about your player
    renderPlayerInformationWindow(display_w,display_h);

    ImGui::Render();
}
#endif