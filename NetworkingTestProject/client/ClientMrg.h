#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <thread>
#include "./Client.h"
#include "./OnUpdate.h"
#include "./ClientEventListener/ClientEventListener.h"
#include "./ImGuiRenderContent.h"
#include "./../Common/Utils/Logger.h"
#include "./../Common/Utils/TileManager/MapAndGridGenerator.h"
#include "./../Common/Utils/MainGameLoop/MainGameLoop.h"
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
#include "./../Common/Utils/GlobalConfigurations.h"
#include "./../Common/Objects/Spells/SpellFactory.h"
// Window dimensions

GLFWwindow* window;

/*
* ----------------------------------
*     Sprite Diminesion Info
* ----------------------------------
*/
// Width and height of the entire texture
float textureWidth  = 1920.0f;
float textureHeight = 1080.0f;

float onePixelWidth  = 1 / textureWidth;
float onePixelHeight = 1 / textureHeight;

// Width and height of the sub-texture
float subTextureWidth  = 64.0f;
float subTextureHeight = 64.0f;

//Location of the sub-texture (Starting from top-left)
float row    = 0.0f;
float column = 1.0f;

// Calculate texture coordinates for the sub-texture
float texCoordLeft   = (subTextureWidth * column) / textureWidth;               // Left edge of the sub-texture
float texCoordRight  = ((column + 1) * subTextureWidth) / textureWidth;         // Right edge of the sub-texture
float texCoordTop    = 1.0f - ((subTextureHeight* row) / textureHeight);        // Top edge of the sub-texture
float texCoordBottom = 1.0f - (((row + 1)*(subTextureHeight)) / textureHeight); // Bottom edge of the sub-texture

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








glm::mat4 Vec3ToMat4(const glm::vec3& translation) {
    return glm::translate(glm::mat4(1.0f), translation);
}

/*
*This is the central location of all of the client information and connections
* 
*Host of the main renderloop
*/
class ClientMrg {

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;

    std::unique_ptr<VertexArray> m_BlockedTileVAO;
    std::unique_ptr<VertexBuffer> m_BlockedTileVertexBuffer;
    std::unique_ptr<IndexBuffer> m_BlockedTileIndexBuffer;
    std::unique_ptr<Shader> m_BlockedTileShader;


    std::unique_ptr<VertexArray> m_PlayerVAO;
    std::unique_ptr<VertexBuffer> m_PlayerVertexBuffer;
    std::unique_ptr<IndexBuffer> m_PlayerIndexBuffer;
    std::unique_ptr<Shader> m_PlayerShader;

    std::unique_ptr<VertexArray> m_MapVAO;
    std::unique_ptr<VertexBuffer> m_MapVertexBuffer;
    std::unique_ptr<IndexBuffer> m_MapIndexBuffer;
    std::unique_ptr<Shader> m_MapShader;
    std::unique_ptr<Texture> m_MApTexture;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;

    std::unique_ptr<VertexArray> m_GridVAO;
    std::unique_ptr<VertexBuffer> m_GridVertexBuffer;
    std::unique_ptr<Shader> m_GridShader;

    std::unique_ptr<VertexArray> m_CoursorVAO;
    std::unique_ptr<VertexBuffer> m_CoursorVertexBuffer;

    glm::mat4 projection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight * aspectRatio, projHeight * aspectRatio, -1.0f, 1.0f);
    ClientConnection client;
    ClientEventListener eventListener;

    std::vector<float> m_Gridvertices;

    int dynamicLightingFlag = 0;

    long long previousTime = getCurrentTimeInMillis();


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

    void genGrid() {
        float gridSize = GlobalConfigurations::getInstance().getScale();  // Size of each grid spot
        std::vector<float> Gridvertices;
        int mapWidth = m_MApTexture->GetWidth();
        int mapHeight = m_MApTexture->GetHeigth();
        

        generateGrid(mapWidth, mapHeight, gridSize, gridSize, Gridvertices);
        m_GridShader = std::make_unique<Shader>("./OpenGL/resources/Grid.shader", 2);
        m_GridVAO = std::make_unique<VertexArray>();
        m_GridVertexBuffer = std::make_unique<VertexBuffer>(Gridvertices.data(), Gridvertices.size() * sizeof(float));
        VertexBufferLayout gridLayout;
        gridLayout.Push<float>(3); // Position
        m_GridVAO->AddBuffer(*m_GridVertexBuffer, gridLayout);

        m_Gridvertices = Gridvertices;
    }




    void StartOpenGL() {
        PlayerDatabase::getInstance().getPlayer(playerName).addSpell(SpellFactory::getInstance().createMagicMissle());
        PlayerDatabase::getInstance().getPlayer(playerName).addSpell(SpellFactory::getInstance().createFireball());
        PlayerDatabase::getInstance().getPlayer(playerName).addSpell(SpellFactory::getInstance().createCloudOfDaggers());
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

        m_MApTexture = std::make_unique<Texture>("./OpenGL/resources/map2.jpg");
        int mapWidth =m_MApTexture->GetWidth();
        int mapHeight =m_MApTexture->GetHeigth();

        /*---------------------------------------------------------------------------------------
        *               Code to generate grid NEED TO MOVE TO OWN CLASS
        ---------------------------------------------------------------------------------------*/
        genGrid();

        //-----------------------------------------------------------------------------------------
        m_MapShader = std::make_unique<Shader>("./OpenGL/resources/Map.shader",2);

        GLfloat mapVertices[] = {
            //   Position            Texture coordinates
                -(float)(mapWidth/2), -(float)(mapHeight / 2), 0.0f,  0.0f,0.0f,       // 0
                (float)(mapWidth / 2), -(float)(mapHeight / 2), 0.0f,  1.0f,0.0f,       // 1
                 (float)(mapWidth / 2),  (float)(mapHeight / 2), 0.0f,  1.0f,1.0f,       // 2
                -(float)(mapWidth / 2),  (float)(mapHeight / 2), 0.0f,  0.0f,1.0f        // 3
        };
        unsigned int mapIndices[] = {
            0,1,2,
            2,3,0
        };




        m_MapVAO = std::make_unique<VertexArray>();
        m_MapVertexBuffer = std::make_unique<VertexBuffer>(mapVertices, sizeof(mapVertices));
        VertexBufferLayout mapLayout;
        mapLayout.Push<float>(3);
        mapLayout.Push<float>(2);
        m_MapVAO->AddBuffer(*m_MapVertexBuffer, mapLayout);
        m_MapIndexBuffer = std::make_unique<IndexBuffer>(mapIndices, 6);




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
        
        m_PlayerVAO = std::make_unique<VertexArray>();
        m_PlayerVertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout playerlayout;
        playerlayout.Push<float>(3); //Position
        playerlayout.Push<float>(2); //Texture Coords
        m_PlayerVAO->AddBuffer(*m_PlayerVertexBuffer, playerlayout);
        m_PlayerIndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
        m_Texture = std::make_unique<Texture>("./OpenGL/resources/PixelArtTest2.png");
        m_PlayerShader = std::make_unique<Shader>("./OpenGL/resources/Player.shader", 1);


        std::thread updateThread(OnUpdateThread);
        updateThread.detach();
        GLuint ssbo;

       
        m_Shader = std::make_unique<Shader>("./OpenGL/resources/Basic.shader", 1);

        int numOfParticles = 8000;
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, ((numOfParticles)*(16)) * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(3); //Position
        layout.Push<float>(2); //Texture Coords
        layout.Push<float>(3); //Translation
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, ((numOfParticles)*(6)));


        //The Map Tile Information should be initialized by this point
        m_BlockedTileShader = std::make_unique<Shader>("./OpenGL/resources/BlockedTile.shader",1);
        int numOfTiles = GlobalConfigurations::getInstance().getMapTileInformation().size();
        m_BlockedTileVAO = std::make_unique<VertexArray>();
        m_BlockedTileVertexBuffer = std::make_unique<VertexBuffer>(nullptr, ((numOfTiles) * (16)) * sizeof(float));
        VertexBufferLayout BlockedTilelayout;
        BlockedTilelayout.Push<float>(3); //Position
        BlockedTilelayout.Push<float>(4); //Color
        m_BlockedTileVAO->AddBuffer(*m_BlockedTileVertexBuffer, BlockedTilelayout);
        m_BlockedTileIndexBuffer = std::make_unique<IndexBuffer>(nullptr, ((numOfTiles) * (6)));




        MainGameLoop::getInstance().executeInitialtive();
        while (!glfwWindowShouldClose(window))
        {
            long long tmpTime = getCurrentTimeInMillis();
            float delta = static_cast<float>(tmpTime-previousTime);
            previousTime = tmpTime;
            OnUpdate(delta);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            MainGameLoop::getInstance().checkGameState();

            Renderer renderer;
            //Interface with the event listeners
            eventListener.processInput(window);
            projection = glm::ortho(-projWidth * aspectRatio, projWidth * aspectRatio, -projHeight, projHeight, -1.0f, 1.0f);
            glm::mat4 MapModel = Vec3ToMat4(glm::vec3(1.0f, 1.0f, 1.0f));
      
            std::vector<float> testInput = ParticleDatabase::getInstance().getParticlesRAW();
            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Map                                             *
             *----------------------------------------------------------------------------------------------*/
            m_MapShader->Bind();
            if (dynamicLightingFlag == 1) {
                GLuint bindingPoint = 0; 
                glGenBuffers(1, &ssbo);
                size_t bufferSize = testInput.size() * (sizeof(float));
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
                glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, testInput.data(), GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, ssbo);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // Unbind SSBO


                 // Should match the binding specified in the shader
                GLuint shader = 0;
                GLuint ssboIndex = glGetProgramResourceIndex(shader, GL_SHADER_STORAGE_BLOCK, "ParticlesBuffer");
                glShaderStorageBlockBinding(shader, ssboIndex, bindingPoint);
                m_MapShader->SetUniform1i("numberOfLights", testInput.size());
            }
            m_MApTexture->Bind();
            m_MapShader->SetUniform1i("u_texture", 0);
            m_MapShader->SetUniform1i("isDynamicLightingOn", dynamicLightingFlag);
            m_MapShader->SetUniformMat4f("translation", MapModel);
            m_MapShader->SetUniformMat4f("projection", projection);
            m_MapShader->SetUniformMat4f("view", view);
            renderer.Draw(*m_MapVAO, *m_MapIndexBuffer, *m_MapShader);
            

            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Grid                                            *
             *----------------------------------------------------------------------------------------------*/

            glm::mat4 GridModel = Vec3ToMat4(glm::vec3(1.0f,1.0f,1.0f));
            glm::vec4 gridColor = GlobalConfigurations::getInstance().getGridColor();

            m_GridShader->Bind();
            m_GridShader->SetUniform4f("u_color", gridColor.r, gridColor.g, gridColor.b,gridColor.w);
            m_GridShader->SetUniformMat4f("translation", GridModel);
            m_GridShader->SetUniformMat4f("projection", projection);
            m_GridShader->SetUniformMat4f("view", view);
            renderer.DrawGrid(*m_GridVAO, *m_GridShader, m_Gridvertices.size()/3);


            std::vector<float> tmpCoursorPos = eventListener.getCursor();

            glm::vec4 cursorColor = GlobalConfigurations::getInstance().getCursorColor();

            m_CoursorVAO = std::make_unique<VertexArray>();
            m_CoursorVertexBuffer = std::make_unique<VertexBuffer>(tmpCoursorPos.data(), tmpCoursorPos.size() * sizeof(float));
            m_GridShader->SetUniform4f("u_color", cursorColor.r, cursorColor.g, cursorColor.b, cursorColor.w);
            VertexBufferLayout coursorLayout;
            coursorLayout.Push<float>(3); // Position
            m_CoursorVAO->AddBuffer(*m_CoursorVertexBuffer, coursorLayout);
            renderer.DrawGrid(*m_CoursorVAO, *m_GridShader, tmpCoursorPos.size() / 3);


            /*----------------------------------------------------------------------------------------------*
             *                                   Render The Players                                         *
             *----------------------------------------------------------------------------------------------*/
          

            //Draw each player to the screen
            
            for (auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
                Player player = PlayerDatabase::getInstance().getPlayer(playerEntry.first);
                glm::mat4 transTest = Vec3ToMat4(glm::vec3(player.getPositionX()+1.0f, player.getPositionY()+1.0f, player.getPositionZ()));
                transTest = glm::scale(transTest,glm::vec3(GlobalConfigurations::getInstance().getScale()));
                m_PlayerShader->Bind();
                m_Texture->Bind();
                m_PlayerShader->SetUniformMat4f("translation", transTest);
                m_PlayerShader->SetUniformMat4f("projection", projection);
                m_PlayerShader->SetUniform1i("u_Texture", 0);
                m_PlayerShader->SetUniformMat4f("view", view);
                renderer.Draw(*m_PlayerVAO, *m_PlayerIndexBuffer, *m_PlayerShader);
              
            }
            
            /*----------------------------------------------------------------------------------------------*
             *                                   Render Particles                                           *
             *----------------------------------------------------------------------------------------------*/

            std::vector<float> tmpVerticies = ParticleDatabase::getInstance().getVerticies();
            std::vector<unsigned int> tmpIndicies = ParticleDatabase::getInstance().getIndicies();
            if (!(tmpVerticies.empty())) {
                m_VertexBuffer->Bind();
                glBufferData(GL_ARRAY_BUFFER, tmpVerticies.size() * sizeof(float), tmpVerticies.data(), GL_DYNAMIC_DRAW); // Orphan the buffer and allocate new data
                m_VertexBuffer->UnBind();

                m_IndexBuffer->Bind();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, tmpIndicies.size() * sizeof(unsigned int), tmpIndicies.data(), GL_DYNAMIC_DRAW); // Orphan the buffer and allocate new data
                m_IndexBuffer->UnBind();


                m_Shader->Bind();
                m_Shader->SetUniformMat4f("projection", projection);
                m_Shader->SetUniform1i("u_Texture", 0);
                m_Shader->SetUniformMat4f("view", view);
                renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
            }

                
            
            std::vector<float> blockTileTmpVertices = GlobalConfigurations::getInstance().getBlockedTilesVertexArray();
            std::vector<unsigned int> blockTileTmpIndicies = GlobalConfigurations::getInstance().getBlockedTileIndiceArray();
            if (!(blockTileTmpVertices.empty())) {
                m_BlockedTileVertexBuffer->Bind();
                glBufferData(GL_ARRAY_BUFFER, blockTileTmpVertices.size() * sizeof(float), blockTileTmpVertices.data(), GL_DYNAMIC_DRAW); // Orphan the buffer and allocate new data
                m_BlockedTileVertexBuffer->UnBind();

                m_BlockedTileIndexBuffer->Bind();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, blockTileTmpIndicies.size() * sizeof(unsigned int), blockTileTmpIndicies.data(), GL_DYNAMIC_DRAW); // Orphan the buffer and allocate new data
                m_BlockedTileIndexBuffer->UnBind();


                m_BlockedTileShader->Bind();
                m_BlockedTileShader->SetUniformMat4f("projection", projection);
                m_BlockedTileShader->SetUniformMat4f("view", view);
                renderer.Draw(*m_BlockedTileVAO, *m_BlockedTileIndexBuffer, *m_BlockedTileShader);
            }





            //Render ImGUI menu
            ImGui_ImplGlfwGL3_NewFrame();
            imGuiRender(window,dynamicLightingFlag, eventListener);
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