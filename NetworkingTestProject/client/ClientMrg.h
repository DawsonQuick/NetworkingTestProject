#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "./Client.h"
#include "./ClientEventListener.h"
#include "./../Common/Utils/Logger.h"
#include "./../OpenGL/Utils/Renderer.h"
#include "./../OpenGL/vendor/imgui/imgui.h"
#include "./../OpenGL/vendor/glm/glm.hpp"
#include "./../OpenGL/vendor/glm/gtc/matrix_transform.hpp"
#include "./../OpenGL/Utils/VertexBuffer.h"
#include "./../OpenGL/Utils/VertexBufferLayout.h"
#include "./../OpenGL/Utils/Texture.h"
#include "./../OpenGL/vendor/glm/glm.hpp"
#include "./../OpenGL/vendor/glm/gtc/matrix_transform.hpp"
#include "./../OpenGL/vendor/imgui/imgui.h"
#include "./../OpenGL/vendor/imgui/imgui_impl_glfw_gl3.h"

// Window dimensions

GLFWwindow* window;




/*---------------------------------------------------------------------------------------
*               Code to generate grid NEED TO MOVE TO OWN CLASS
 ---------------------------------------------------------------------------------------*/

void generateGrid(float maxSize, float gridSize, std::vector<float>& vertices) {
    // Calculate the number of tiles along one side of the grid
    int numTiles = static_cast<int>(maxSize / gridSize);

    // Generate horizontal lines
    for (int i = 0; i <= numTiles; ++i) {
        float y = -maxSize / 2.0f + i * gridSize;
        vertices.push_back(-maxSize / 2.0f); // x-coordinate of the start point
        vertices.push_back(y);               // y-coordinate of the start point
        vertices.push_back(0.0f);            // z-coordinate
        vertices.push_back(maxSize / 2.0f);  // x-coordinate of the end point
        vertices.push_back(y);               // y-coordinate of the end point
        vertices.push_back(0.0f);            // z-coordinate
    }

    // Generate vertical lines
    for (int i = 0; i <= numTiles; ++i) {
        float x = -maxSize / 2.0f + i * gridSize;
        vertices.push_back(x);               // x-coordinate of the start point
        vertices.push_back(-maxSize / 2.0f); // y-coordinate of the start point
        vertices.push_back(0.0f);            // z-coordinate
        vertices.push_back(x);               // x-coordinate of the end point
        vertices.push_back(maxSize / 2.0f);  // y-coordinate of the end point
        vertices.push_back(0.0f);            // z-coordinate
    }
}
//---------------------------------------------------------------------------------------

void generateGridQuads(float maxSize, float gridSize, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    // Calculate the number of tiles along one side of the grid
    int numTiles = static_cast<int>(maxSize / gridSize);

    // Generate quads in the middle of each grid square
    for (int i = 0; i < numTiles; ++i) {
        for (int j = 0; j < numTiles; ++j) {
            float x = -maxSize / 2.0f + j * gridSize;
            float y = -maxSize / 2.0f + i * gridSize;
            float quadSize = gridSize * 0.5f; // Half the size of each grid square
            // Define vertices for the quad
            vertices.push_back(x - quadSize); // Bottom-left
            vertices.push_back(y - quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x + quadSize); // Bottom-right
            vertices.push_back(y - quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x + quadSize); // Top-right
            vertices.push_back(y + quadSize);
            vertices.push_back(0.0f);
            vertices.push_back(x - quadSize); // Top-left
            vertices.push_back(y + quadSize);
            vertices.push_back(0.0f);
            // Define indices for the quad
            unsigned int baseIndex = static_cast<unsigned int>(vertices.size() / 3) - 4; // Index of the first vertex of the quad
            indices.push_back(baseIndex);     // Bottom-left
            indices.push_back(baseIndex + 1); // Bottom-right
            indices.push_back(baseIndex + 2); // Top-right
            indices.push_back(baseIndex);     // Bottom-left
            indices.push_back(baseIndex + 2); // Top-right
            indices.push_back(baseIndex + 3); // Top-left
        }
    }
}





/*
* ----------------------------------
*     Sprite Diminesion Info
* ----------------------------------
*/
// Width and height of the entire texture
float textureWidth = 1920.0f;
float textureHeight = 1080.0f;

float onePixelWidth = 1 / textureWidth;
float onePixelHeight = 1 / textureHeight;
// Width and height of the sub-texture
float subTextureWidth = 64.0f;
float subTextureHeight = 64.0f;
float row = 0.0f;
float column = 1.0f;

// Calculate texture coordinates for the sub-texture
float texCoordLeft = (subTextureWidth* column) / textureWidth; // Left edge of the sub-texture
float texCoordRight = ((column +1)*subTextureWidth) / textureWidth; // Right edge of the sub-texture
float texCoordTop = 1.0f - ((subTextureHeight* row)/ textureHeight); // Top edge of the sub-texture
float texCoordBottom = 1.0f - (((row +1)*(subTextureHeight)) / textureHeight); // Bottom edge of the sub-texture

GLfloat vertices[] = {
//   Position            Texture coordinates
    -0.5f, -0.5f, 0.0f,  texCoordLeft  + onePixelWidth,  texCoordBottom + onePixelHeight,       // 0
     0.5f, -0.5f, 0.0f,  texCoordRight - onePixelWidth,  texCoordBottom + onePixelHeight,       // 1
     0.5f,  0.5f, 0.0f,  texCoordRight - onePixelWidth,  texCoordTop    - onePixelHeight,       // 2
    -0.5f,  0.5f, 0.0f,  texCoordLeft  + onePixelWidth,  texCoordTop    - onePixelHeight        // 3
};
unsigned int indices[] = {
    0,1,2,
    2,3,0
};
/*
* ----------------------------------
*/


//Updates all players positions based on if the player has a key pressed, distance moved is determined by that players movement speed
void UpdatePlayerPosition(std::string name) {
    KeyPress keyPress = PlayerDatabase::getInstance().getPlayer(name).getKeyPress();
    Position prevPos = PlayerDatabase::getInstance().getPlayer(name).getPosition();
    double playerSpeed = PlayerDatabase::getInstance().getPlayer(name).getMovementSpeed();
    Position newPos = prevPos;
    if (keyPress.keyW) {
        newPos.Y = prevPos.Y + playerSpeed;
    }
    if (keyPress.keyA) {
        newPos.X = prevPos.X - playerSpeed;
    }
    if (keyPress.keyS) {
        newPos.Y = prevPos.Y - playerSpeed;
    }
    if (keyPress.keyD) {
        newPos.X = prevPos.X + playerSpeed;
    }
    
    //TODO: Add logic for colision detection, 
    //If player hits something, set to previous position
    PlayerDatabase::getInstance().getPlayer(name).setPosition(newPos.X, newPos.Y, newPos.Z, getCurrentTimeInMillis());
}


glm::mat4 Vec3ToMat4(const glm::vec3& translation) {
    return glm::translate(glm::mat4(1.0f), translation);
}

class ClientMrg {

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;


    std::unique_ptr<VertexArray> m_MapVAO;
    std::unique_ptr<VertexBuffer> m_MapVertexBuffer;
    std::unique_ptr<IndexBuffer> m_MapIndexBuffer;
    std::unique_ptr<Shader> m_MapShader;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    std::unique_ptr<VertexArray> m_GridVAO;
    std::unique_ptr<VertexBuffer> m_GridVertexBuffer;
    std::unique_ptr<Shader> m_GridShader;

    glm::mat4 projection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight * aspectRatio, projHeight * aspectRatio, -1.0f, 1.0f);
    ClientConnection client;
    ClientEventListener eventListener;

    int dynamicLightingFlag = 0;


public:
    ClientMrg(): client("127.0.0.1", 8080), eventListener(client, nullptr) {
        /*
        * -----------Start Back End Processing------------
        */
            client.Start();
            playerName = client.getPlayerName();
        /*
        * ------------------------------------------------
        */
    }




    void StartOpenGL() {
        // Init GLFW
        glfwInit();
        // Set all the required options for GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        // Create a GLFWwindow object that we can use for GLFW's functions
        window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
        glfwMakeContextCurrent(window);
        eventListener = ClientEventListener(client, window);

        // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
        glewExperimental = GL_TRUE;
        // Initialize GLEW to setup the OpenGL Function pointers
        glewInit();


        /*---------------------------------------------------------------------------------------
        *               Code to generate grid NEED TO MOVE TO OWN CLASS
        ---------------------------------------------------------------------------------------*/
        float maxSize = 500.0f; // Maximum size of the grid
        float gridSize = 1.0f;  // Size of each grid spot
        std::vector<float> Gridvertices;
        generateGrid(maxSize, gridSize, Gridvertices);
        m_GridShader = std::make_unique<Shader>("./OpenGL/resources/Grid.shader", 2);
        m_GridVAO = std::make_unique<VertexArray>();
        m_GridVertexBuffer = std::make_unique<VertexBuffer>(Gridvertices.data(), Gridvertices.size() * sizeof(float));
        VertexBufferLayout gridLayout;
        gridLayout.Push<float>(3); // Position
        m_GridVAO->AddBuffer(*m_GridVertexBuffer, gridLayout);
        //-----------------------------------------------------------------------------------------
        std::vector<float> mapVertices;
        std::vector<unsigned int> mapIndicies;
        generateGridQuads(maxSize, gridSize, mapVertices, mapIndicies);
        m_MapShader = std::make_unique<Shader>("./OpenGL/resources/Map.shader",5);
        m_MapVAO = std::make_unique<VertexArray>();
        m_MapVertexBuffer = std::make_unique<VertexBuffer>(mapVertices.data(), mapVertices.size()*sizeof(float));
        VertexBufferLayout mapLayout;
        mapLayout.Push<float>(3);
        m_MapVAO->AddBuffer(*m_MapVertexBuffer, mapLayout);
        m_MapIndexBuffer = std::make_unique<IndexBuffer>(mapIndicies.data(), mapIndicies.size());

        // Define the viewport dimensions
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glfwSwapInterval(1); //Set framerate equal to system framerate (v-sync)

     
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, false);
        ImGui::StyleColorsDark();
        m_Shader = std::make_unique<Shader>("./OpenGL/resources/Basic.shader", 1);
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3); //Position
        layout.Push<float>(2); //Texture Coords
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_Texture = std::make_unique<Texture>("./OpenGL/resources/PixelArtTest2.png");
        
        m_VertexBuffer->UnBind();
        m_IndexBuffer->UnBind();
        m_VAO->UnBind();

        while (!glfwWindowShouldClose(window))
        {
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            Renderer renderer;
            //Interface with the event listeners
            eventListener.processInput(window);
            projection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight, projHeight, -1.0f, 1.0f);
            glm::mat4 MapModel = Vec3ToMat4(glm::vec3(1.0f, 1.0f, 1.0f));
            //TODO: Draw BackGround Textures

            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Map                                             *
             *----------------------------------------------------------------------------------------------*/
            m_MapShader->Bind();
            int index = 0;
            if (dynamicLightingFlag == 1) {
                for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
                    m_MapShader->SetUniform3f("lightPos[" + std::to_string(index) + "]", playerEntry.second.getPositionX(), playerEntry.second.getPositionY(), playerEntry.second.getPositionZ());
                    index++;
                }
                m_MapShader->SetUniform1i("numberOfLights", index);
            }
            m_MapShader->SetUniform1i("isDynamicLightingOn", dynamicLightingFlag);
            m_MapShader->SetUniformMat4f("translation", MapModel);
            m_MapShader->SetUniformMat4f("projection", projection);
            m_MapShader->SetUniformMat4f("view", view);
            renderer.Draw(*m_MapVAO, *m_MapIndexBuffer, *m_MapShader);


            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Grid                                            *
             *----------------------------------------------------------------------------------------------*/

            //TODO: Draw Spacing Grid
            glm::mat4 GridModel = Vec3ToMat4(glm::vec3(1.0f,1.0f,1.0f));
            m_GridShader->Bind();
            m_GridShader->SetUniformMat4f("translation", GridModel);
            m_GridShader->SetUniformMat4f("projection", projection);
            m_GridShader->SetUniformMat4f("view", view);
            renderer.DrawGrid(*m_GridVAO, *m_GridShader, Gridvertices.size()/3);


            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Players                                         *
             *----------------------------------------------------------------------------------------------*/
            //Update all player positions
            for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) { UpdatePlayerPosition(playerEntry.first); }

            //Draw each player to the screen
            for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
                Player player = PlayerDatabase::getInstance().getPlayer(playerEntry.first);
                glm::mat4 transTest = Vec3ToMat4(glm::vec3(player.getPositionX(), player.getPositionY(), player.getPositionZ()));
                
                m_Shader->Bind();
                m_Texture->Bind();
                m_Shader->SetUniformMat4f("translation", transTest);
                m_Shader->SetUniformMat4f("projection", projection);
                m_Shader->SetUniform1i("u_Texture", 0);
                m_Shader->SetUniformMat4f("view", view);
                renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
              
            }

            //Render ImGUI menu
            ImGui_ImplGlfwGL3_NewFrame();
            ImGui::Begin("Test");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Checkbox("Dynamic Lighting", reinterpret_cast<bool*>(&dynamicLightingFlag));
            for(auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
                ImGui::Text("Players: %s",playerEntry.first.c_str());
                ImGui::Text("X: %f Y: %f",playerEntry.second.getPositionX(),playerEntry.second.getPositionY());
                ImGui::Text("KeyBoard Press: ");
                ImGui::Text("W:");ImGui::SameLine();
                if (playerEntry.second.getKeyPress().keyW) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor();}
                else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
                ImGui::Text("A:"); ImGui::SameLine();
                if (playerEntry.second.getKeyPress().keyA) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor();}
                else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor();}
                ImGui::Text("S:"); ImGui::SameLine();
                if (playerEntry.second.getKeyPress().keyS) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed"); ImGui::PopStyleColor();}
                else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }
                ImGui::Text("D:"); ImGui::SameLine();
                if (playerEntry.second.getKeyPress().keyD) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); ImGui::Text("Pressed");ImGui::PopStyleColor(); }
                else { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); ImGui::Text("Not Pressed"); ImGui::PopStyleColor(); }

                
            }
            ImGui::SliderFloat("lagSimulationTime (ms)", &lagSimulationTime, 0.0f, 100.0f);
            ImGui::SliderFloat("Zoom", &projHeight, 15.0f, 100.0f);
            ImGui::SliderFloat("Zoom", &projWidth, 15.0f, 100.0f);
            ImGui::Text("Camera Position:");
            ImGui::Text("X: %f", eventListener.getCameraXTranslation());
            ImGui::Text("Y: %f", eventListener.getCameraYTranslation());
            ImGui::End();
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            // Swap the screen buffers
            glfwSwapBuffers(window);
            // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();
        }

        ImGui_ImplGlfwGL3_Shutdown();
        ImGui::DestroyContext();
        ClientDisconnectMessage msg(MessageType::CLIENTDISCONNECT, 3, getCurrentTimeInSeconds(), playerName);
        std::string msgTest = msg.serialize();
        const char* msgToSend = msgTest.c_str();
        client.sendMsg(msgToSend);
        //TODO: Send a disconnect message letting the server know this client is disconnecting

    }
};