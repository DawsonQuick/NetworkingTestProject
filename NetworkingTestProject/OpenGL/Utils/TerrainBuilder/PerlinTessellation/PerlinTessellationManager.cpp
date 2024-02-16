#include "PerlinTessellationManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "./../../stb_image.h"
const float verticesFormat[] = {

            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0, 0.0, 0.0,//0
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f, 0.0, 0.0, 0.0,//1
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0, 0.0, 0.0,//2
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0, 0.0, 0.0,//3

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0, 0.0, 0.0,//4
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0, 0.0, 0.0,//5
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f, 0.0, 0.0, 0.0,//6
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f, 0.0, 0.0, 0.0,//7

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//8
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//9
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//10
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//11

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//12
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//13
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//14
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f, 0.0, 0.0, 0.0,//15

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f, 0.0, 0.0, 0.0,//16
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f, 0.0, 0.0, 0.0,//17
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0, 0.0, 0.0,//18
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0, 0.0, 0.0,//19

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f, 0.0, 0.0, 0.0,//20
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f, 0.0, 0.0, 0.0,//21
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0, 0.0, 0.0,//22
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f, 0.0, 0.0, 0.0,//23


};

const unsigned int indicesFormat[] = {
    0,1,2,
    2,3,0,
    4,5,6,
    6,7,4,
    8,9,10,
    10,11,8,
    12,13,14,
    14,15,12,
    16,17,18,
    18,19,16,
    20,21,22,
    22,23,20
};




PerlinTessellationManager::PerlinTessellationManager():
    m_ThreadLock()
{

}
PerlinTessellationManager::~PerlinTessellationManager() {

}
PerlinTessellationOutputs PerlinTessellationManager::Generate(PerlinTessellationInputs inputs) {
    std::lock_guard<std::mutex> lock(m_ThreadLock);
    m_isThreadLocked = true;              //Enable thread lock so that no other threads can run functions that use the global variables till they are repopulated
    m_TerrainHeight = inputs.height;
    m_TerrainWidth = inputs.width;
    std::cout << inputs.height<<" " << inputs.width<<" "<< inputs.seed<<" "<<inputs.rez<< std::endl;
    PerlinTessellationOutputs output;
    
    float* verticesArray(new float[inputs.rez * inputs.rez * 20]);
    float* heightMap(new float[inputs.width * inputs.height]);
    float* derivativeMap(new float[inputs.width * inputs.height]);

    auto start_time = std::chrono::high_resolution_clock::now();

    m_PerlinNoise = std::make_unique<PerlinNoiseTessellation>(inputs.rez, inputs.width, inputs.height, verticesArray, heightMap, derivativeMap, inputs.seed);

    m_derivativeArray = std::make_unique<float[]>(inputs.width * inputs.height);
    for (int i = 0; i < (inputs.width * inputs.height); ++i) {
        m_derivativeArray[i] = derivativeMap[i];
    }

    m_heightMapArray = std::make_unique<float[]>(inputs.width * inputs.height);
    for (int i = 0; i < (inputs.width * inputs.height); ++i) {
        m_heightMapArray[i] = heightMap[i];
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution outside time: " << duration_ms.count() << " milliseconds" << std::endl;


    // Bind the texture and set its data
    unsigned int lightSourcetextureMap;
    glGenTextures(1, &lightSourcetextureMap);
    glBindTexture(GL_TEXTURE_2D, lightSourcetextureMap); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, inputs.width, inputs.height, 0, GL_RED, GL_FLOAT, heightMap);

    glGenerateMipmap(GL_TEXTURE_2D);
    output.m_HeightMapID = lightSourcetextureMap;
    unsigned int derivativeMapId;
    glGenTextures(1, &derivativeMapId);
    glBindTexture(GL_TEXTURE_2D, derivativeMapId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, inputs.width, inputs.height, 0, GL_RED, GL_FLOAT, derivativeMap);

    glGenerateMipmap(GL_TEXTURE_2D);
    output.m_DerivativeMapID = derivativeMapId;
    // first, configure the cube's VAO (and terrainVBO)
    unsigned int terrainVBO;
    glGenVertexArrays(1, &output.m_TerrainVAO);
    glBindVertexArray(output.m_TerrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (inputs.rez * inputs.rez * 20), verticesArray, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind the VAO to prevent modification


    std::cout << "Inside m_TerrainVAO: " << output.m_TerrainVAO << std::endl;

    delete[] verticesArray;
    delete[] heightMap;
    delete[] derivativeMap;

    auto SceneObjects_start_time = std::chrono::high_resolution_clock::now();
    std::vector<Vertex> grassVertices;
    std::vector<unsigned int> grassIndices;
    m_viableSpawningLocations = std::make_unique<std::vector<glm::vec3>>();
    for (int i = 0; i < inputs.width; i++) {
        for (int j = 0; j < inputs.height; j++) {
            int index = i * inputs.width + j;
            if (m_derivativeArray[index] < 0.00000001) {
                m_viableSpawningLocations->push_back(glm::vec3(j, m_heightMapArray[index], i));
            }
        }
    }

    // Print the size of the vector
    std::cout << "Number of instances:  " << m_viableSpawningLocations->size() << std::endl;


    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("./resources/PineTreeLowQuality.glb", aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenSmoothNormals);
    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        const aiMesh* mesh = scene->mMeshes[meshIndex];
        if (mesh->HasTextureCoords(0)) {
            std::cout << "Found Texture for mesh at position : " << meshIndex << std::endl;
        }
    }


    for (int copyIndex = 0; copyIndex < m_viableSpawningLocations->size(); ++copyIndex) {
        float valueX = m_viableSpawningLocations->at(copyIndex).x;
        float valueY = m_viableSpawningLocations->at(copyIndex).y + 30;
        float valueZ = m_viableSpawningLocations->at(copyIndex).z;


        // Assume a single mesh for simplicity (you may need to loop through multiple meshes)
        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
            const aiMesh* mesh = scene->mMeshes[meshIndex];
            // Store the starting index for each mesh
            unsigned int baseIndex = grassVertices.size();

            // Extract vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
                Vertex vertex;
                // Extract position
                vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                // Extract normal
                vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                // Extract texture coordinates (if available)
                if (mesh->HasTextureCoords(0)) {
                    vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
              
                }
                else {
                    vertex.texCoords = glm::vec2(0.0f, 0.0f);
                }

                vertex.translation = glm::vec3(valueX, valueY, valueZ);

                grassVertices.push_back(vertex);
            }

            // Extract indices
            for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                const aiFace& face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                    grassIndices.push_back(baseIndex + face.mIndices[j]);
                }
            }
        }
    }
    // Load texture for the current mesh material
    aiMaterial* material = scene->mMaterials[scene->mMeshes[0]->mMaterialIndex];
    aiString texturePath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
        std::cout << "Getting here" << std::endl;
        unsigned int meshTextureID;
        glGenTextures(1, &meshTextureID);
        glBindTexture(GL_TEXTURE_2D, meshTextureID);

        int textureWidth, textureHeight, textureChannels;
        unsigned char* textureData = stbi_load("./resources/Image_0.png", &textureWidth, &textureHeight, &textureChannels, 4);
        if (textureData) {
            // Set the texture data and generate mipmaps
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
            glGenerateMipmap(GL_TEXTURE_2D);

            std::cout << "Loaded texture map of size " << textureWidth << " x " << textureHeight << std::endl;
        }
        else {
            std::cout << "Failed to load texture map" << std::endl;
        }
        stbi_image_free(textureData);
        output.m_MeshTextureID = meshTextureID;
    }

    std::cout << "Total vertices size: " << grassVertices.size() << std::endl;

    auto AssigningData_begin = std::chrono::high_resolution_clock::now();
    //Define the Vertex Array Object and Vertex Buffer
    output.m_SceneElementsVAO = std::make_unique<VertexArray>();
    output.m_SceneElementsVBO = std::make_unique<VertexBuffer>(grassVertices.data(), sizeof(Vertex) * grassVertices.size());

    //Define and populate the Vertex Buffer Layout
    VertexBufferLayout layout;
    layout.Push<float>(3); //verticies positions
    layout.Push<float>(2); //Texture Mapping
    layout.Push<float>(3); //Normal Mapping
    layout.Push<float>(3); //Translation mapping
    output.m_SceneElementsVAO->AddBuffer(*output.m_SceneElementsVBO, layout); //Pipe the layout to the Vertex Array

    //Define and populate the Index Buffer
    output.m_SceneElementsIBO = std::make_unique<IndexBuffer>(grassIndices.data(), grassIndices.size());


    auto AssigningData_end = std::chrono::high_resolution_clock::now();
    auto Assignment_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(AssigningData_end - AssigningData_begin);
    std::cout << "Total time to Assign Buffers" << Assignment_duration_ms.count() << " milliseconds" << std::endl;

    auto SceneObjects_end_time = std::chrono::high_resolution_clock::now();
    auto SceneObjectsGeneration_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(SceneObjects_end_time - SceneObjects_start_time);
    std::cout << "Total time to generate sceneobjects" << SceneObjectsGeneration_duration_ms.count() << " milliseconds" << std::endl;
    m_isThreadLocked = false;

    m_viableSpawningLocations.reset();
    return output;
}

PerlinRegenOutputs PerlinTessellationManager::RePopulateBuffers(CameraInfo camInfo , std::string type , int minRegenDistance , int maxRegenRange) {
    std::lock_guard<std::mutex> lock(m_ThreadLock);
    if (!m_isThreadLocked) {
        auto SceneObjects_start_time = std::chrono::high_resolution_clock::now();
        std::vector<Vertex> grassVertices;
        std::vector<unsigned int> grassIndices;
        m_viableSpawningLocations = std::make_unique<std::vector<glm::vec3>>();
        for (int i = 0; i < m_TerrainWidth; i++) {
            for (int j = 0; j < m_TerrainHeight; j++) {
                int index = i * m_TerrainWidth + j;
                if (m_derivativeArray[index] < 0.00000001) {
                    float distance = std::sqrt(std::pow((camInfo.m_CameraPos.x - j), 2) + std::pow((camInfo.m_CameraPos.z - i), 2));
                    if (distance >= minRegenDistance && distance < maxRegenRange) {
                        m_viableSpawningLocations->push_back(glm::vec3(j, m_heightMapArray[index], i));
                    }
                }
            }
        }

        // Print the size of the vector
        std::cout << "Number of instances:  " << m_viableSpawningLocations->size() << std::endl;

        auto generateVerticiesandIndices_start_time = std::chrono::high_resolution_clock::now();
        Assimp::Importer importer;
        const aiScene* scene;
        if (type == "HIGHRES") {
            scene = importer.ReadFile("./resources/pine_tree_01.glb", aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenSmoothNormals);
        }
        else if (type == "LOWRES") {
            scene = importer.ReadFile("./resources/PineTreeLowQuality.glb", aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenSmoothNormals);
        }
        else {
            scene = importer.ReadFile("./resources/pine_tree_01.glb", aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenSmoothNormals);
        }



        for (int copyIndex = 0; copyIndex < m_viableSpawningLocations->size(); ++copyIndex) {
            float valueX = m_viableSpawningLocations->at(copyIndex).x;
            float valueY = m_viableSpawningLocations->at(copyIndex).y + 30;
            float valueZ = m_viableSpawningLocations->at(copyIndex).z;


            // Assume a single mesh for simplicity (you may need to loop through multiple meshes)
            for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
                const aiMesh* mesh = scene->mMeshes[meshIndex];
                // Store the starting index for each mesh
                unsigned int baseIndex = grassVertices.size();

                // Extract vertices
                for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
                    Vertex vertex;
                    // Extract position
                    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
                    // Extract normal
                    vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
                    // Extract texture coordinates (if available)
                    if (mesh->HasTextureCoords(0)) {
                        vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

                    }
                    else {
                        vertex.texCoords = glm::vec2(0.0f, 0.0f);
                    }

                    vertex.translation = glm::vec3(valueX, valueY, valueZ);

                    grassVertices.push_back(vertex);
                }

                // Extract indices
                for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
                    const aiFace& face = mesh->mFaces[i];
                    for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                        grassIndices.push_back(baseIndex + face.mIndices[j]);
                    }
                }
            }
        }
        auto generateVerticiesandIndices_stop_time = std::chrono::high_resolution_clock::now();
        auto generateVerticiesandIndices_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(generateVerticiesandIndices_stop_time - generateVerticiesandIndices_start_time);
        std::cout << "Total time to generate Verticies and Indicies sceneobjects" << generateVerticiesandIndices_duration_ms.count() << " milliseconds" << std::endl;

        PerlinRegenOutputs output;
        output.Vertices = grassVertices;
        output.Indices = grassIndices;

        auto SceneObjects_end_time = std::chrono::high_resolution_clock::now();
        auto SceneObjectsGeneration_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(SceneObjects_end_time - SceneObjects_start_time);
        std::cout << "Total time to RE-generate sceneobjects" << SceneObjectsGeneration_duration_ms.count() << " milliseconds" << std::endl;
        m_isThreadLocked = false;
        m_viableSpawningLocations.reset();
        return output;
    }
    else {
        //Add a default return value of function is thread locked
    }

}
float PerlinTessellationManager::getHeightAt(float x, float z) {
    int index = static_cast<int>(std::round(z)) * m_TerrainWidth + static_cast<int>(std::round(x));
    return  m_heightMapArray[index]+10;
 }
