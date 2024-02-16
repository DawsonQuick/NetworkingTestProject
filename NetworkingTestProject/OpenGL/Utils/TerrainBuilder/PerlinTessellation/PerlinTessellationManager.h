#pragma once
#include <vector>
#include <string>
#include <memory>
#include "./Generation/PerlinNoiseTessellation.h"
#include "../../VertexArray.h"
#include "../../IndexBuffer.h"
#include "./../../VertexBufferLayout.h"
#include "./../../Camera/Camera.h"
#include <chrono>
#include <iostream>
#include "./../../../vendor/glm/glm.hpp"

#define GLEW_STATIC
#include <GL/glew.h>

#include <mutex>
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 translation;
};
struct PerlinTessellationInputs {
	unsigned int width;
	unsigned int height;
	unsigned int rez;
	unsigned int seed;
};

struct PerlinTessellationOutputs {
	unsigned int m_TerrainVAO;
	unsigned int m_HeightMapID;
	unsigned int m_DerivativeMapID;
	unsigned int m_MeshTextureID;
	std::unique_ptr<VertexArray> m_SceneElementsVAO;
	std::unique_ptr<VertexBuffer> m_SceneElementsVBO;
	std::unique_ptr<IndexBuffer> m_SceneElementsIBO;
};

struct PerlinRegenOutputs {
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	PerlinRegenOutputs() = default;
};

class PerlinTessellationManager {
public:
	PerlinTessellationManager();
	~PerlinTessellationManager();
	PerlinTessellationOutputs Generate(PerlinTessellationInputs inputs);
	PerlinRegenOutputs RePopulateBuffers(CameraInfo camInfo, std::string type, int minRegenDistance , int maxRegenRange);
	float getHeightAt(float x, float z);
private:

	std::mutex m_ThreadLock;
	bool m_isThreadLocked;
	unsigned int m_TerrainWidth, m_TerrainHeight;
	std::unique_ptr<PerlinNoiseTessellation> m_PerlinNoise;
	std::unique_ptr<float[]> m_derivativeArray;
	std::unique_ptr<float[]> m_heightMapArray;
	std::unique_ptr<std::vector<glm::vec3>> m_viableSpawningLocations;
	
};