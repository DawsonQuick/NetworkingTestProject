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

// Window dimensions
const GLuint WIDTH = 500, HEIGHT = 500;
GLFWwindow* window;

/*
* ----------------------------------
*     Sprite Diminesion Info
* ----------------------------------
*/
GLfloat vertices[] = {

    -0.5f,-0.5f, 0.0f,   // 0
     0.5f,-0.5f, 0.0f,   // 1
     0.5f, 0.5f, 0.0f,   // 2
    -0.5f, 0.5f, 0.0f,   // 3

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

class ClientMrg {

private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    glm::mat4 projection = glm::ortho(-projWidth, projWidth, -projHeight, projHeight, -1.0f, 1.0f);
    ClientConnection client;
    ClientEventListener eventListener;


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

        // Define the viewport dimensions
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glfwSwapInterval(1); //Set framerate equal to system framerate (v-sync)

     
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_Shader = std::make_unique<Shader>("./OpenGL/resources/Basic.shader", 1);
        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader->Bind();
        m_VertexBuffer->UnBind();
        m_IndexBuffer->UnBind();
        m_VAO->UnBind();

        while (!glfwWindowShouldClose(window))
        {
            projection = glm::ortho(-projWidth, projWidth, -projHeight, projHeight, -1.0f, 1.0f);
            glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            eventListener.processInput(window);
            for (const auto& playerEntry : PlayerDatabase::getInstance().getPlayers()) {
                Player player = playerEntry.second;
                glm::mat4 transTest = Vec3ToMat4(glm::vec3(player.getPositionX(), player.getPositionY(), player.getPositionZ()));
                Renderer renderer;
                {
                    m_Shader->Bind();
                    m_Shader->SetUniformMat4f("translation", transTest);
                    m_Shader->SetUniformMat4f("projection", projection);
                    //TODO add a view Matrix that is updated when the player clicks and drags with the mouse
                    renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
                }
            }
            // Swap the screen buffers
            glfwSwapBuffers(window);
            // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
            glfwPollEvents();
        }
        ClientDisconnectMessage msg(MessageType::CLIENTDISCONNECT, 3, getCurrentTimeInSeconds(), playerName);
        std::string msgTest = msg.serialize();
        const char* msgToSend = msgTest.c_str();
        client.sendMsg(msgToSend);
        //TODO: Send a disconnect message letting the server know this client is disconnecting

    }
};