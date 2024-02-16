#include "SkyBoxManager.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>



float skyboxVertices[] = {
    // positions          
    -10000.0f,  10000.0f, -10000.0f,
    -10000.0f, -10000.0f, -10000.0f,
     10000.0f, -10000.0f, -10000.0f,
     10000.0f, -10000.0f, -10000.0f,
     10000.0f,  10000.0f, -10000.0f,
    -10000.0f,  10000.0f, -10000.0f,

    -10000.0f, -10000.0f,  10000.0f,
    -10000.0f, -10000.0f, -10000.0f,
    -10000.0f,  10000.0f, -10000.0f,
    -10000.0f,  10000.0f, -10000.0f,
    -10000.0f,  10000.0f,  10000.0f,
    -10000.0f, -10000.0f,  10000.0f,

     10000.0f, -10000.0f, -10000.0f,
     10000.0f, -10000.0f,  10000.0f,
     10000.0f,  10000.0f,  10000.0f,
     10000.0f,  10000.0f,  10000.0f,
     10000.0f,  10000.0f, -10000.0f,
     10000.0f, -10000.0f, -10000.0f,

    -10000.0f, -10000.0f,  10000.0f,
    -10000.0f,  10000.0f,  10000.0f,
     10000.0f,  10000.0f,  10000.0f,
     10000.0f,  10000.0f,  10000.0f,
     10000.0f, -10000.0f,  10000.0f,
    -10000.0f, -10000.0f,  10000.0f,

    -10000.0f,  10000.0f, -10000.0f,
     10000.0f,  10000.0f, -10000.0f,
     10000.0f,  10000.0f,  10000.0f,
     10000.0f,  10000.0f,  10000.0f,
    -10000.0f,  10000.0f,  10000.0f,
    -10000.0f,  10000.0f, -10000.0f,

    -10000.0f, -10000.0f, -10000.0f,
    -10000.0f, -10000.0f,  10000.0f,
     10000.0f, -10000.0f, -10000.0f,
     10000.0f, -10000.0f, -10000.0f,
    -10000.0f, -10000.0f,  10000.0f,
     10000.0f, -10000.0f,  10000.0f
};
unsigned int loadCubemap(std::vector<std::string> faces);
SkyBoxManager::SkyBoxManager() {
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &m_SkyBoxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(m_SkyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> faces
    {
        "./resources/TessellationRefactorResources/skybox/right.jpg",
        "./resources/TessellationRefactorResources/skybox/left.jpg",
        "./resources/TessellationRefactorResources/skybox/top.jpg",
        "./resources/TessellationRefactorResources/skybox/bottom.jpg",
        "./resources/TessellationRefactorResources/skybox/front.jpg",
        "./resources/TessellationRefactorResources/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    std::cout << "Cube Map Texure ID: " << cubemapTexture << std::endl;

}

SkyBoxManager::~SkyBoxManager() {

}
void SkyBoxManager::RenderSkyBox(Shader shader , glm::mat4 m_Veiw, glm::mat4 m_Proj) {
    glDepthFunc(GL_LEQUAL);
    shader.Bind();
    shader.SetUniformMat4f("view", m_Veiw);
    shader.SetUniformMat4f("projection", m_Proj);
    shader.SetUniform1i("skybox", 0);

    // skybox cube
    glBindVertexArray(m_SkyBoxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 6);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    std::cout << "SkyBox Texture id: " << textureID << std::endl;
    return textureID;
}