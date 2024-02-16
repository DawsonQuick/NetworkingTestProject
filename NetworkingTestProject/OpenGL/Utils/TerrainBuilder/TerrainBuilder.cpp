#include "TerrainBuilder.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
TerrainBuilder::TerrainBuilder(unsigned int m_rez, int* width, int* height, std::vector<float>* vertices, const std::string& heightMap, int heightMapID, const std::string& textureMap, int textureMapId) {
   

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + heightMapID);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int nrChannels;
    unsigned char* data = stbi_load(heightMap.c_str(), width, height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);


        std::cout << "Loaded heightmap of size " << &height << " x " << &width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    unsigned int textureMapVariable;
    glGenTextures(textureMapId, &textureMapVariable);
    glActiveTexture(GL_TEXTURE0+textureMapId);
    glBindTexture(GL_TEXTURE_2D, textureMapVariable); 

    int width2, height2, nrChannels2;
    unsigned char* data2 = stbi_load(textureMap.c_str(), &width2, &height2, &nrChannels2, 4);
    if (data2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);


        std::cout << "Loaded texture map of size " << width2 << " x " << height2 << std::endl;
    }
    else {
        std::cout << "Failed to load texture map" << std::endl;
    }
    stbi_image_free(data2);


    for (unsigned i = 0; i <= m_rez - 1; i++)
    {
        for (unsigned j = 0; j <= m_rez - 1; j++)
        {
            vertices->push_back(-*width / 2.0f + *width * i / (float)m_rez); // v.x
            vertices->push_back(0.0f); // v.y
            vertices->push_back(-*height / 2.0f + *height * j / (float)m_rez); // v.z
            vertices->push_back(i / (float)m_rez); // u
            vertices->push_back(j / (float)m_rez); // v

            vertices->push_back(-*width / 2.0f + *width * (i + 1) / (float)m_rez); // v.x
            vertices->push_back(0.0f); // v.y
            vertices->push_back(-*height / 2.0f + *height * j / (float)m_rez); // v.z
            vertices->push_back((i + 1) / (float)m_rez); // u
            vertices->push_back(j / (float)m_rez); // v

            vertices->push_back(-*width / 2.0f + *width * i / (float)m_rez); // v.x
            vertices->push_back(0.0f); // v.y
            vertices->push_back(-*height / 2.0f + *height * (j + 1) / (float)m_rez); // v.z
            vertices->push_back(i / (float)m_rez); // u
            vertices->push_back((j + 1) / (float)m_rez); // v

            vertices->push_back(-*width / 2.0f + *width * (i + 1) / (float)m_rez); // v.x
            vertices->push_back(0.0f); // v.y
            vertices->push_back(-*height / 2.0f + *height * (j + 1) / (float)m_rez); // v.z
            vertices->push_back((i + 1) / (float)m_rez); // u
            vertices->push_back((j + 1) / (float)m_rez); // v
        }
    }

    std::cout << "Loaded " << m_rez * m_rez << " patches of 4 control points each" << std::endl;
    std::cout << "Processing " << m_rez * m_rez * 4 << " vertices in vertex shader" << std::endl;
}

TerrainBuilder::~TerrainBuilder() {

}